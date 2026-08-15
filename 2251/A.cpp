// CF 2251A interactive
// Score-aware scheduler: SLO1/SLO2, w_tp/w_c, tp_base/tp_UB/dist_base drive
// urgency, batch size, edge family choice, and prefill split; UP/DOWN are
// independent FIFOs overlapping compute; cloud pin fixed at P PRE.

#include <bits/stdc++.h>
using namespace std;

struct {
  int K;
  double S, latency, bw;
  long long bpt;
  int layers;
} SYS;

struct {
  double SLO1, SLO2, tp_UB, tp_base, dist_base, w_tp, w_c;
} SP;

struct {
  map<int, double> ppre, pproc, ppost, dpre, dproc, dpost;
  static void put(map<int, double> &m, int sz, double v) {
    if (v >= 0)
      m[sz] = v;
  }
  void add_row(int bs, double a, double b, double c, double d, double e,
               double f) {
    put(ppre, bs, a);
    put(pproc, bs, b);
    put(ppost, bs, c);
    put(dpre, bs, d);
    put(dproc, bs, e);
    put(dpost, bs, f);
  }
  static double get(const map<int, double> &m, int size) {
    if (m.empty())
      return 0;
    auto it = m.find(size);
    if (it != m.end())
      return it->second;
    auto hi = m.lower_bound(size);
    if (hi == m.begin())
      return hi->second;
    if (hi == m.end())
      return prev(hi)->second;
    auto lo = prev(hi);
    return lo->second + (hi->second - lo->second) * (double(size) - lo->first) /
                            (hi->first - lo->first);
  }
  double pre_pre(int Lin) const { return get(ppre, Lin); }
  double pre_proc_full(int Lin) const { return get(pproc, Lin); }
  double pre_post(int Lin) const { return get(ppost, Lin); }
  double dec_pre(int m) const { return get(dpre, m); }
  double dec_proc(int m) const { return get(dproc, m); }
  double dec_post(int m) const { return get(dpost, m); }
  double pre_proc_piece(int Lin, int ls, int le) const {
    if (SYS.layers <= 0 || le <= ls)
      return 0;
    return pre_proc_full(Lin) * double(le - ls) / double(SYS.layers);
  }
} TAB;

static double xfer_ms(long long len) {
  return SYS.latency + 8.0 * double(len) * double(SYS.bpt) / (SYS.bw * 1e6);
}

enum St {
  None,
  NeedPPre,
  WaitPPre,
  WaitUpPre,
  NeedPProc,
  WaitPProc,
  WaitDownPre,
  NeedPPost,
  WaitPPost,
  NeedDPre,
  WaitDPre,
  WaitUpDec,
  NeedDProc,
  WaitDProc,
  WaitDownDec,
  NeedDPost,
  WaitDPost,
  Finished
};

struct Req {
  St st = None;
  int Lin = 0, cid = -1, ls = 0;
  double arrival = 0;
  double last_token_t = -1; // for TPOT
  bool tdr_done = false;
};

static vector<Req> R;
static bool edge_busy;
static vector<char> cloud_busy;
static double now_t = 0;
static double edge_free_at = 0;
static vector<double> cloud_free_at;

// Independent duplex FIFOs (lengths in tokens / group sizes).
static deque<long long> up_q, down_q;

// Live score stats (feed SLO / w_tp / w_c / tp_* logic).
static double first_arrival = -1;
static long long tokens_done = 0;
static double sum_tdr = 0;
static int n_tdr = 0;
static double sum_tpot = 0;
static int n_tpot = 0;

// Runtime policy derived from SP + live metrics (contest score geometry).
struct {
  double w_tp = 0.5, w_c = 0.5;
  double excess_tdr = 0, excess_tpot = 0, dist = 0;
  double tp = 0, tp_comp = 0, wait_comp = 1;
  double score_est = 0; // w_tp*tp_comp + w_c*wait_comp
  double urge_tdr = 1;  // unit vector * waiting pressure
  double urge_tpot = 1;
  double urge_tp = 1;
  double urge_wait = 1;
  int max_batch = 1;
  bool prefer_admit = false;
} POL;

static string strip(string s) {
  while (!s.empty() && (s.back() == '\r' || s.back() == ' '))
    s.pop_back();
  return s;
}

static vector<string> tok(const string &line) {
  vector<string> t;
  stringstream ss(line);
  string x;
  while (ss >> x)
    t.push_back(x);
  return t;
}

static void ensure(int rid) {
  if (rid < 0)
    return;
  if (rid >= (int)R.size())
    R.resize(rid + 1);
}

static bool valid_rid(int rid) { return rid >= 0 && rid < (int)R.size(); }

static void push_up(long long len) { up_q.push_back(len); }
static void push_down(long long len) { down_q.push_back(len); }

static void pop_up() {
  if (!up_q.empty())
    up_q.pop_front();
}
static void pop_down() {
  if (!down_q.empty())
    down_q.pop_front();
}

static double up_done_at() {
  double t = now_t;
  for (long long len : up_q)
    t += xfer_ms(len);
  return t;
}
static double down_done_at() {
  double t = now_t;
  for (long long len : down_q)
    t += xfer_ms(len);
  return t;
}
static double up_head_done_at() {
  if (up_q.empty())
    return now_t;
  return now_t + xfer_ms(up_q.front());
}
static double down_head_done_at() {
  if (down_q.empty())
    return now_t;
  return now_t + xfer_ms(down_q.front());
}

static bool up_busy() { return !up_q.empty(); }
static bool down_busy() { return !down_q.empty(); }

static int table_max_batch() {
  int mx = 1;
  for (auto &kv : TAB.dpre)
    mx = max(mx, kv.first);
  for (auto &kv : TAB.dproc)
    mx = max(mx, kv.first);
  for (auto &kv : TAB.dpost)
    mx = max(mx, kv.first);
  return min(mx, 2000);
}

// Pick decode batch size using table + score weights.
// High w_tp → larger batches. High w_c / TPOT excess → smaller (protect SLO2).
static int choose_batch(int ready_n) {
  if (ready_n <= 1)
    return 1;
  int cap = min(ready_n, min(table_max_batch(), max(1, POL.max_batch)));
  auto cost_per = [&](int m) {
    return (SYS.S + TAB.dec_pre(m) + SYS.S + TAB.dec_proc(m) + SYS.S +
            TAB.dec_post(m) + xfer_ms(m) + xfer_ms(m)) /
           m;
  };
  int best = 1;
  double best_c = 1e300;
  for (int m = 1; m <= cap; ++m) {
    double c = cost_per(m);
    // Keep factor > 0: tp wants larger m, waiting/TPOT wants smaller m.
    double lg = log2(1.0 + m);
    double factor = (1.0 + POL.w_c * POL.urge_tpot * lg) /
                    (1.0 + POL.w_tp * POL.urge_tp * lg);
    c *= factor;
    // Soft cap: batch wall-time vs SLO2 (hurts TPOT axis of dist).
    double wall = SYS.S + TAB.dec_pre(m) + xfer_ms(m) + SYS.S +
                  TAB.dec_proc(m) + xfer_ms(m) + SYS.S + TAB.dec_post(m);
    if (wall > SP.SLO2)
      c *= 1.0 + POL.w_c * POL.urge_tpot * (wall / max(SP.SLO2, 1e-9) - 1.0);
    if (c < best_c) {
      best_c = c;
      best = m;
    }
  }
  return max(1, best);
}

static double clamp01(double x, double base, double target) {
  if (fabs(target - base) < 1e-15)
    return 0;
  return max(0.0, min(1.0, (x - base) / (target - base)));
}

// Contest waiting distance:
//   excess_tdr  = max(0, (tdr  - SLO1) / SLO1)
//   excess_tpot = max(0, (tpot - SLO2) / SLO2)
//   dist = sqrt(excess_tdr^2 + excess_tpot^2)
// wait_comp = 1 - dist/dist_base (clamped), or special case dist_base==0
// Blend finished samples with live provisional waits so policy reacts early.
static void refresh_policy() {
  POL.w_tp = SP.w_tp;
  POL.w_c = SP.w_c;
  POL.max_batch = table_max_batch();
  POL.prefer_admit = false;

  double acc_tdr = sum_tdr;
  int cnt_tdr = n_tdr;
  double acc_tpot = sum_tpot;
  int cnt_tpot = n_tpot;
  for (int i = 0; i < (int)R.size(); ++i) {
    const Req &r = R[i];
    if (r.st == None || r.st == Finished)
      continue;
    if (!r.tdr_done) {
      acc_tdr += now_t - r.arrival;
      ++cnt_tdr;
    } else if (r.last_token_t >= 0) {
      acc_tpot += now_t - r.last_token_t;
      ++cnt_tpot;
    }
  }

  double tdr = cnt_tdr > 0 ? acc_tdr / cnt_tdr : 0;
  double tpot = cnt_tpot > 0 ? acc_tpot / cnt_tpot : 0;
  double slo1 = max(SP.SLO1, 1e-9);
  double slo2 = max(SP.SLO2, 1e-9);

  POL.excess_tdr = cnt_tdr > 0 ? max(0.0, (tdr - SP.SLO1) / slo1) : 0;
  POL.excess_tpot = cnt_tpot > 0 ? max(0.0, (tpot - SP.SLO2) / slo2) : 0;
  POL.dist = hypot(POL.excess_tdr, POL.excess_tpot);

  if (SP.dist_base > 0)
    POL.wait_comp = max(0.0, 1.0 - POL.dist / SP.dist_base);
  else
    POL.wait_comp = (POL.dist == 0 ? 1.0 : 0.0);

  // Waiting pressure grows as wait_comp falls; split along excess unit vector.
  POL.urge_wait = 1.0 + SP.w_c * (1.0 - POL.wait_comp);
  if (POL.dist > 1e-12) {
    POL.urge_tdr = POL.urge_wait * (POL.excess_tdr / POL.dist);
    POL.urge_tpot = POL.urge_wait * (POL.excess_tpot / POL.dist);
  } else {
    // No excess yet: mild equal bias so both SLOs stay in the loop.
    POL.urge_tdr = 0.5 * POL.urge_wait;
    POL.urge_tpot = 0.5 * POL.urge_wait;
  }
  // Tiny floor only — do not flatten the vector when one axis dominates.
  POL.urge_tdr = max(POL.urge_tdr, 0.05 * max(SP.w_c, 1e-9));
  POL.urge_tpot = max(POL.urge_tpot, 0.05 * max(SP.w_c, 1e-9));

  if (first_arrival >= 0 && now_t > first_arrival && tokens_done > 0)
    POL.tp = double(tokens_done) / (now_t - first_arrival);
  else
    POL.tp = 0;
  POL.tp_comp = clamp01(POL.tp, SP.tp_base, SP.tp_UB);
  POL.urge_tp = 1.0 + SP.w_tp * (1.0 - POL.tp_comp);
  POL.score_est = SP.w_tp * POL.tp_comp + SP.w_c * POL.wait_comp;

  // Admit only when throughput is weak and waiting budget still has room.
  if (POL.tp_comp < 0.45 && POL.wait_comp > 0.25)
    POL.prefer_admit = true;
  if (POL.tp_comp <= 0 && tokens_done > 0 && POL.wait_comp > 0.1)
    POL.prefer_admit = true;

  // Batch cap from which score axis is hurting wait_comp more.
  if (POL.excess_tpot > POL.excess_tdr && POL.excess_tpot > 0)
    POL.max_batch = max(1, POL.max_batch / 2);
  else if (POL.prefer_admit || POL.tp_comp < 0.5)
    POL.max_batch = table_max_batch();
}

static double cloud_backlog(int c) {
  // Compute only — links do not occupy the cloud.
  double w = max(0.0, cloud_free_at[c] - now_t);
  for (int i = 0; i < (int)R.size(); ++i) {
    if (R[i].cid != c || R[i].st == None || R[i].st == Finished)
      continue;
    if (R[i].st == NeedPProc || R[i].st == WaitPProc) {
      int left = SYS.layers - R[i].ls;
      if (left > 0)
        w += SYS.S + TAB.pre_proc_piece(R[i].Lin, R[i].ls, SYS.layers);
    } else if (R[i].st == NeedDProc || R[i].st == WaitDProc) {
      w += SYS.S + TAB.dec_proc(1);
    } else if (R[i].st == WaitUpDec || R[i].st == WaitUpPre) {
      w += 0.2 * TAB.dec_proc(1);
    }
  }
  return w;
}

static double est_new_prefill_on_cloud(int Lin, int c) {
  // Rough path after P PRE: UP + full P PROC + DOWN + P POST (edge POST
  // separate).
  return cloud_backlog(c) + (up_done_at() - now_t) + xfer_ms(Lin) + SYS.S +
         TAB.pre_proc_full(Lin) + xfer_ms(Lin);
}

static int pick_cloud_for_admit() {
  if (SYS.K <= 0)
    return 0;
  int best = 0;
  double best_b = 1e300;
  for (int c = 0; c < SYS.K; ++c) {
    double b = cloud_backlog(c);
    // Prefer clouds that can absorb UP sooner (overlap with other work).
    b += 0.15 * (up_done_at() - now_t);
    if (b < best_b - 1e-12 || (fabs(b - best_b) < 1e-12 && c < best)) {
      best_b = b;
      best = c;
    }
  }
  return best;
}

// Lower sort key = schedule sooner. Per-request contribution to dist axes.
static double req_urgency(int rid) {
  if (!valid_rid(rid))
    return 0;
  const Req &r = R[rid];
  double slo1 = max(SP.SLO1, 1e-9);
  double slo2 = max(SP.SLO2, 1e-9);
  double u = 0;
  if (!r.tdr_done) {
    double age = now_t - r.arrival;
    double ex = max(0.0, (age - slo1) / slo1);
    u += SP.w_c * POL.urge_tdr * (age / slo1 + 2.0 * ex);
  }
  if (r.tdr_done && r.last_token_t >= 0) {
    double gap = now_t - r.last_token_t;
    double ex = max(0.0, (gap - slo2) / slo2);
    u += SP.w_c * POL.urge_tpot * (gap / slo2 + 2.0 * ex);
  } else if (r.tdr_done) {
    u += SP.w_tp * POL.urge_tp + 0.25 * SP.w_c * POL.urge_tpot;
  }
  u += 1e-9 * rid;
  return -u;
}

static void sort_urgent(vector<int> &ids) {
  sort(ids.begin(), ids.end(),
       [&](int a, int b) { return req_urgency(a) < req_urgency(b); });
}

// Family score for edge: lower wins. Score-geometry driven (no hard stage
// ranks).
static double family_score(int kind, const vector<int> &ids) {
  if (ids.empty())
    return 1e300;
  double head = -req_urgency(ids[0]);
  double avg = 0;
  for (int id : ids)
    avg += -req_urgency(id);
  avg /= ids.size();
  double mix = 0.6 * head + 0.4 * avg;

  if (kind == 0) { // DPost → tokens (tp) + closes TPOT gaps
    return -(SP.w_tp * POL.urge_tp * 1.5 + SP.w_c * POL.urge_tpot) * mix;
  }
  if (kind == 1) { // PPost → closes TDR
    return -(SP.w_c * POL.urge_tdr * 1.75) * mix;
  }
  if (kind == 2) { // DPre → starts decode path
    double link_pen = 0;
    if (up_busy())
      link_pen +=
          0.15 * POL.urge_tp * ((up_done_at() - now_t) / max(SP.SLO2, 1e-9));
    return -(SP.w_tp * POL.urge_tp + 0.65 * SP.w_c * POL.urge_tpot) * mix +
           link_pen;
  }
  // PPre → admit only if it helps tp without wrecking wait_comp / TDR.
  int id = ids[0];
  int c = pick_cloud_for_admit();
  double path = est_new_prefill_on_cloud(R[id].Lin, c);
  double age = now_t - R[id].arrival;
  double rem = SP.SLO1 - age;
  double admit = POL.prefer_admit ? SP.w_tp * POL.urge_tp : 0.15 * SP.w_tp;
  double risk = 0;
  if (rem > 0 && path > rem)
    risk += SP.w_c * POL.urge_tdr * (path / rem);
  else if (rem <= 0)
    risk += SP.w_c * POL.urge_tdr * (1.0 + path / max(SP.SLO1, 1e-9));
  if (POL.wait_comp < 0.35)
    risk += SP.w_c * POL.urge_wait * (1.0 - POL.wait_comp);
  return -admit * mix + risk + 0.05;
}

// Split prefill to overlap cloud compute with duplex links / decode.
static int choose_piece_le(int rid) {
  if (!valid_rid(rid))
    return SYS.layers;
  int ls = max(0, min(R[rid].ls, SYS.layers));
  int rem = SYS.layers - ls;
  if (rem <= 1)
    return SYS.layers;

  int c = R[rid].cid;
  bool need_decode = false;
  bool decode_inflight = false;
  for (int i = 0; i < (int)R.size(); ++i) {
    if (i == rid || R[i].cid != c)
      continue;
    St s = R[i].st;
    if (s == NeedDProc)
      need_decode = true;
    if (s == WaitUpDec || s == WaitDProc || s == WaitDownDec)
      decode_inflight = true;
  }

  // TDR dominates dist and no decode waiting → finish this prefill.
  if (POL.excess_tdr >= POL.excess_tpot && POL.excess_tdr > 0 && !need_decode)
    return SYS.layers;

  // Nothing to overlap with → full piece.
  if (!need_decode && !decode_inflight && !up_busy() && !down_busy())
    return SYS.layers;

  double full = TAB.pre_proc_full(R[rid].Lin);
  double target = TAB.dec_proc(1);
  if (need_decode)
    target = min(target, TAB.dec_proc(1) * 0.75);
  if (up_busy())
    target = min(target, max(1e-6, up_head_done_at() - now_t));
  if (down_busy())
    target = min(target, max(1e-6, down_head_done_at() - now_t));
  target = max(target, xfer_ms(1));

  // tp pressure → longer chunks; TPOT/decode pressure → shorter.
  double scale = 0.45 + 0.55 * min(2.0, POL.urge_tp) / (1.0 + POL.urge_tpot);
  if (need_decode)
    scale *= 0.6;
  target *= scale;

  int chunk =
      max(1, (int)llround(double(SYS.layers) * target / max(full, 1e-9)));
  chunk = min(chunk, rem);
  if (POL.excess_tdr > POL.excess_tpot)
    chunk = max(chunk, (rem + 1) / 2);
  if (rem <= chunk + 1)
    return SYS.layers;
  return min(SYS.layers, ls + chunk);
}

static void on_arr(int rid, int Lin) {
  if (rid < 0)
    return;
  ensure(rid);
  if (!valid_rid(rid))
    return;
  R[rid] = Req{};
  R[rid].st = NeedPPre;
  R[rid].Lin = Lin;
  R[rid].cid = -1;
  R[rid].ls = 0;
  R[rid].arrival = now_t;
  R[rid].last_token_t = -1;
  R[rid].tdr_done = false;
  if (first_arrival < 0)
    first_arrival = now_t;
}

static void on_fin(int rid) {
  if (rid < 0)
    return;
  ensure(rid);
  if (!valid_rid(rid))
    return;
  R[rid].st = Finished;
}

static void free_server(const string &server) {
  if (server == "E") {
    edge_busy = false;
    edge_free_at = now_t;
  } else if (server.size() >= 2 && server[0] == 'C') {
    int c = stoi(server.substr(1));
    if (0 <= c && c < SYS.K) {
      cloud_busy[c] = 0;
      cloud_free_at[c] = now_t;
    }
  }
}

static void on_tdn(const vector<string> &t) {
  if (t.size() < 4)
    return;
  free_server(t[1]);
  string kind = t[2], step = t[3];

  if (kind == "P" && step == "PRE") {
    if (t.size() < 6)
      return;
    int rid = stoi(t[5]);
    if (rid < 0)
      return;
    ensure(rid);
    if (!valid_rid(rid))
      return;
    if (R[rid].st == WaitPPre) {
      R[rid].st = WaitUpPre;
      push_up(max(1, R[rid].Lin)); // auto UP; independent of DOWN
    }
  } else if (kind == "P" && step == "PROC") {
    if (t.size() < 8)
      return;
    int le = stoi(t[5]), rid = stoi(t[7]);
    if (rid < 0)
      return;
    ensure(rid);
    if (!valid_rid(rid))
      return;
    if (R[rid].st == WaitPProc) {
      R[rid].ls = max(R[rid].ls, le);
      if (le >= SYS.layers) {
        R[rid].st = WaitDownPre;
        push_down(max(1, R[rid].Lin)); // only last piece queues DOWN
      } else {
        R[rid].st = NeedPProc;
      }
    }
  } else if (kind == "P" && step == "POST") {
    if (t.size() < 6)
      return;
    int rid = stoi(t[5]);
    if (rid < 0)
      return;
    ensure(rid);
    if (!valid_rid(rid))
      return;
    if (R[rid].st == WaitPPost) {
      R[rid].st = NeedDPre;
      if (!R[rid].tdr_done) {
        R[rid].tdr_done = true;
        sum_tdr += now_t - R[rid].arrival;
        ++n_tdr;
      }
    }
  } else if (kind == "D" && step == "PRE") {
    if (t.size() < 6)
      return;
    int m = stoi(t[5]);
    if (m < 0 || (int)t.size() < 6 + m)
      return;
    // Per-cloud UPs in increasing cloud index (separate from DOWN).
    map<int, int> per;
    for (int i = 0; i < m; ++i) {
      int rid = stoi(t[6 + i]);
      if (rid < 0)
        continue;
      ensure(rid);
      if (!valid_rid(rid))
        continue;
      if (R[rid].st == WaitDPre) {
        R[rid].st = WaitUpDec;
        if (R[rid].cid >= 0)
          per[R[rid].cid]++;
      }
    }
    for (auto &kv : per)
      push_up(max(1LL, (long long)kv.second));
  } else if (kind == "D" && step == "PROC") {
    if (t.size() < 6)
      return;
    int m = stoi(t[5]);
    if (m < 0 || (int)t.size() < 6 + m)
      return;
    for (int i = 0; i < m; ++i) {
      int rid = stoi(t[6 + i]);
      if (rid < 0)
        continue;
      ensure(rid);
      if (!valid_rid(rid))
        continue;
      if (R[rid].st == WaitDProc)
        R[rid].st = WaitDownDec;
    }
    push_down(max(1, m));
  } else if (kind == "D" && step == "POST") {
    if (t.size() < 6)
      return;
    int m = stoi(t[5]);
    if (m < 0 || (int)t.size() < 6 + m)
      return;
    for (int i = 0; i < m; ++i) {
      int rid = stoi(t[6 + i]);
      if (rid < 0)
        continue;
      ensure(rid);
      if (!valid_rid(rid))
        continue;
      if (R[rid].st == WaitDPost) {
        if (R[rid].last_token_t >= 0) {
          sum_tpot += now_t - R[rid].last_token_t;
          ++n_tpot;
        }
        R[rid].last_token_t = now_t;
        ++tokens_done;
        R[rid].st = NeedDPre;
      }
    }
  }
}

static void on_xdn(const vector<string> &t) {
  if (t.size() < 7)
    return;
  string dir = t[1], phase = t[4];
  if (phase != "PRE" && phase != "DEC")
    return;
  int m = stoi(t[5]);
  if (m < 0 || (int)t.size() < 6 + m)
    return;

  if (dir == "UP")
    pop_up();
  else if (dir == "DOWN")
    pop_down();

  for (int i = 0; i < m; ++i) {
    int rid = stoi(t[6 + i]);
    if (rid < 0)
      continue;
    ensure(rid);
    if (!valid_rid(rid))
      continue;
    if (dir == "UP" && phase == "PRE" && R[rid].st == WaitUpPre)
      R[rid].st = NeedPProc;
    else if (dir == "DOWN" && phase == "PRE" && R[rid].st == WaitDownPre)
      R[rid].st = NeedPPost;
    else if (dir == "UP" && phase == "DEC" && R[rid].st == WaitUpDec)
      R[rid].st = NeedDProc;
    else if (dir == "DOWN" && phase == "DEC" && R[rid].st == WaitDownDec)
      R[rid].st = NeedDPost;
  }
}

static vector<int> collect(St need, int cloud_or_neg) {
  vector<int> v;
  for (int i = 0; i < (int)R.size(); ++i) {
    if (R[i].st != need)
      continue;
    if (cloud_or_neg >= 0 && R[i].cid != cloud_or_neg)
      continue;
    v.push_back(i);
  }
  return v;
}

// Prefer one cloud for D PRE so a single UP transfer feeds a batch D PROC.
static vector<int> best_same_cloud_batch(vector<int> ids) {
  if (ids.empty())
    return ids;
  sort_urgent(ids);
  map<int, vector<int>> by;
  for (int id : ids) {
    if (!valid_rid(id))
      continue;
    int c = R[id].cid;
    if (c < 0)
      c = 0;
    by[c].push_back(id);
  }
  if (by.empty())
    return {};
  int best_c = by.begin()->first;
  double best_v = -1e300;
  for (auto &kv : by) {
    double v = 0;
    for (int id : kv.second)
      v += -req_urgency(id);
    v *= (1.0 + 0.15 * log2(1.0 + kv.second.size()));
    if (v > best_v) {
      best_v = v;
      best_c = kv.first;
    }
  }
  return by[best_c];
}

static void mark_edge_busy(double dur) {
  edge_busy = true;
  edge_free_at = now_t + SYS.S + dur;
}
static void mark_cloud_busy(int c, double dur) {
  cloud_busy[c] = 1;
  cloud_free_at[c] = now_t + SYS.S + dur;
}

static void schedule_and_flush() {
  refresh_policy();
  vector<string> cmds;

  if (!edge_busy) {
    auto dpost = collect(NeedDPost, -1);
    auto ppost = collect(NeedPPost, -1);
    auto dpre = best_same_cloud_batch(collect(NeedDPre, -1));
    auto ppre = collect(NeedPPre, -1);
    sort_urgent(dpost);
    sort_urgent(ppost);
    sort_urgent(dpre);
    sort_urgent(ppre);

    struct Cand {
      double score;
      int kind;
    };
    vector<Cand> cands;
    if (!dpost.empty())
      cands.push_back({family_score(0, dpost), 0});
    if (!ppost.empty())
      cands.push_back({family_score(1, ppost), 1});
    if (!dpre.empty())
      cands.push_back({family_score(2, dpre), 2});
    if (!ppre.empty())
      cands.push_back({family_score(3, ppre), 3});
    sort(cands.begin(), cands.end(),
         [](const Cand &a, const Cand &b) { return a.score < b.score; });

    for (auto &cd : cands) {
      bool did = false;
      if (cd.kind == 0) {
        int m = choose_batch((int)dpost.size());
        string s = "E D POST -1 " + to_string(m);
        for (int i = 0; i < m; ++i) {
          s += " " + to_string(dpost[i]);
          R[dpost[i]].st = WaitDPost;
        }
        cmds.push_back(s);
        mark_edge_busy(TAB.dec_post(m));
        did = true;
      } else if (cd.kind == 1) {
        int id = ppost[0];
        cmds.push_back("E P POST " + to_string(R[id].cid) + " " +
                       to_string(id));
        R[id].st = WaitPPost;
        mark_edge_busy(TAB.pre_post(R[id].Lin));
        did = true;
      } else if (cd.kind == 2) {
        int m = choose_batch((int)dpre.size());
        // Backed-up duplex → smaller D PRE so TPOT axis does not explode.
        double up_left = up_done_at() - now_t;
        double down_left = down_done_at() - now_t;
        if ((up_busy() && up_left > SP.SLO2) ||
            (down_busy() && down_left > SP.SLO2))
          m = min(m, max(1, m / 2));
        if (POL.excess_tpot > POL.excess_tdr && POL.excess_tpot > 0)
          m = min(m, max(1, m / 2));
        string s = "E D PRE -1 " + to_string(m);
        for (int i = 0; i < m; ++i) {
          s += " " + to_string(dpre[i]);
          R[dpre[i]].st = WaitDPre;
        }
        cmds.push_back(s);
        mark_edge_busy(TAB.dec_pre(m));
        did = true;
      } else {
        int id = ppre[0];
        int c = pick_cloud_for_admit();
        R[id].cid = c;
        R[id].ls = 0;
        cmds.push_back("E P PRE " + to_string(c) + " " + to_string(id));
        R[id].st = WaitPPre;
        mark_edge_busy(TAB.pre_pre(R[id].Lin));
        did = true;
      }
      if (did)
        break;
    }
  }

  for (int c = 0; c < SYS.K; ++c) {
    if (cloud_busy[c])
      continue;

    auto dproc = collect(NeedDProc, c);
    auto pproc = collect(NeedPProc, c);
    sort_urgent(dproc);
    sort_urgent(pproc);

    // Score-aware: usually D PROC (tp+tpot); under TDR-dominated dist, finish
    // urgent prefill first if it is clearly hotter than decode.
    bool prefer_pproc = false;
    if (!pproc.empty() && !dproc.empty()) {
      double u_p = -req_urgency(pproc[0]);
      double u_d = -req_urgency(dproc[0]);
      if (POL.excess_tdr > POL.excess_tpot && u_p > u_d * 1.25)
        prefer_pproc = true;
    } else if (!pproc.empty() && dproc.empty()) {
      prefer_pproc = true;
    }

    if (!dproc.empty() && !prefer_pproc) {
      int m = choose_batch((int)dproc.size());
      if (POL.excess_tpot > POL.excess_tdr && POL.excess_tpot > 0)
        m = min(m, max(1, m / 2));
      string s =
          "C" + to_string(c) + " D PROC " + to_string(c) + " " + to_string(m);
      for (int i = 0; i < m; ++i) {
        s += " " + to_string(dproc[i]);
        R[dproc[i]].st = WaitDProc;
      }
      cmds.push_back(s);
      mark_cloud_busy(c, TAB.dec_proc(m));
      continue;
    }

    if (!pproc.empty()) {
      int id = pproc[0];
      int ls = R[id].ls;
      int le = choose_piece_le(id);
      if (ls < le) {
        cmds.push_back("C" + to_string(c) + " P PROC " + to_string(c) + " " +
                       to_string(le) + " " + to_string(ls) + " " +
                       to_string(id));
        R[id].st = WaitPProc;
        mark_cloud_busy(c, TAB.pre_proc_piece(R[id].Lin, ls, le));
      }
    }
  }

  cout << cmds.size() << '\n';
  for (auto &s : cmds)
    cout << s << '\n';
  cout << flush;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  if (!getline(cin, line))
    return 0;
  {
    stringstream ss(strip(line));
    ss >> SYS.K >> SYS.S >> SYS.latency >> SYS.bw >> SYS.bpt >> SYS.layers;
  }
  if (!getline(cin, line))
    return 0;
  {
    stringstream ss(strip(line));
    ss >> SP.SLO1 >> SP.SLO2 >> SP.tp_UB >> SP.tp_base >> SP.dist_base >>
        SP.w_tp >> SP.w_c;
  }
  if (!getline(cin, line))
    return 0;
  int N = stoi(strip(line));
  for (int i = 0; i < N; ++i) {
    if (!getline(cin, line))
      return 0;
    stringstream ss(strip(line));
    int bs;
    double a, b, c, d, e, f;
    ss >> bs >> a >> b >> c >> d >> e >> f;
    TAB.add_row(bs, a, b, c, d, e, f);
  }

  edge_busy = false;
  SYS.K = max(0, SYS.K);
  cloud_busy.assign(SYS.K, 0);
  edge_free_at = 0;
  cloud_free_at.assign(SYS.K, 0);
  up_q.clear();
  down_q.clear();
  first_arrival = -1;
  tokens_done = 0;
  sum_tdr = n_tdr = 0;
  sum_tpot = n_tpot = 0;
  refresh_policy();

  while (true) {
    if (!getline(cin, line))
      return 0;
    line = strip(line);
    if (line.empty())
      continue;
    if (line == "END")
      break;

    now_t = stod(line);
    if (!getline(cin, line))
      return 0;
    int e = stoi(strip(line));
    vector<vector<string>> evs;
    evs.reserve(e);
    for (int i = 0; i < e; ++i) {
      if (!getline(cin, line))
        return 0;
      evs.push_back(tok(strip(line)));
    }

    for (auto &t : evs)
      if (t.size() >= 2 && t[0] == "FIN")
        on_fin(stoi(t[1]));

    for (auto &t : evs) {
      if (t.empty())
        continue;
      if (t[0] == "ARR") {
        if (t.size() >= 3)
          on_arr(stoi(t[1]), stoi(t[2]));
      } else if (t[0] == "TDN")
        on_tdn(t);
      else if (t[0] == "XDN")
        on_xdn(t);
    }

    for (auto &t : evs)
      if (t.size() >= 2 && t[0] == "FIN")
        on_fin(stoi(t[1]));

    schedule_and_flush();
  }
  return 0;
}
