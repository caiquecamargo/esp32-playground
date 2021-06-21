import {
  a as r,
  b as a,
  c as t,
  d as e,
  e as l,
  f as o,
  F as p,
  g as u,
  h as d,
  i as m,
  j as h,
  k as v,
  l as f,
  m as g,
  n as x,
  o as s,
  p as b,
  r as n,
  T as w,
  u as c,
  w as i,
} from "./vendor.js";
!(function (e = ".", t = "__import__") {
  try {
    self[t] = new Function("u", "return import(u)");
  } catch (n) {
    const s = new URL(e, location),
      r = (e) => {
        URL.revokeObjectURL(e.src), e.remove();
      };
    (self[t] = (e) =>
      new Promise((n, a) => {
        const l = new URL(e, s);
        if (self[t].moduleMap[l]) return n(self[t].moduleMap[l]);
        const o = new Blob(
            [`import * as m from '${l}';`, `${t}.moduleMap['${l}']=m;`],
            { type: "text/javascript" }
          ),
          i = Object.assign(document.createElement("script"), {
            type: "module",
            src: URL.createObjectURL(o),
            onerror() {
              a(new Error(`Failed to import: ${e}`)), r(i);
            },
            onload() {
              n(self[t].moduleMap[l]), r(i);
            },
          });
        document.head.appendChild(i);
      })),
      (self[t].moduleMap = {});
  }
})("/assets/");
var _ = e({});
const z = { class: "flex justify-between gap-1" };
var y, M, k, C;
(_.render = function (e, r, a, l, o, i) {
  return s(), t("nav", z, [n(e.$slots, "default")]);
}),
  ((M = y || (y = {})).NORMAL = "normal"),
  (M.REVERSE = "reverse");
class S {
  constructor() {}
  get animation() {
    return this._animation;
  }
  set animation(e) {
    this._animation = e;
  }
  async animate() {
    var e, t;
    return (
      null == (e = this._animation) || e.play(),
      null == (t = this._animation) ? void 0 : t.finished
    );
  }
  stop() {
    var e;
    null == (e = this._animation) || e.pause();
  }
  reverse() {
    var e;
    return null == (e = this._animation) || e.reverse(), this;
  }
  setDirection(e) {
    return (this._animation.direction = e), this;
  }
}
((C = k || (k = {}))[(C.OPENED = 0)] = "OPENED"),
  (C[(C.CLOSED = 1)] = "CLOSED");
class E {
  constructor() {
    this._state = r(1);
  }
  get state() {
    return this._state.value;
  }
  get animation() {
    return this._animation;
  }
  get finished() {
    var e;
    return null == (e = this._animation) ? void 0 : e.animation.finished;
  }
  set animation(e) {
    this._animation = e;
  }
  reverseState() {
    this._state.value = 0 === this._state.value ? 1 : 0;
  }
  isOpened() {
    return 0 === this._state.value;
  }
  isClosed() {
    return 1 === this._state.value;
  }
  async animate() {
    return new Promise(async (e) => {
      var t, n;
      await (null == (t = this._animation) ? void 0 : t.animate()),
        null == (n = this._animation) || n.reverse(),
        e();
    });
  }
  open() {
    var e;
    if (this.isClosed())
      return (
        (this._state.value = 0),
        null == (e = this._animation) || e.setDirection(y.NORMAL),
        this.animate()
      );
  }
  close() {
    var e;
    if (this.isOpened())
      return (
        (this._state.value = 1),
        null == (e = this._animation) || e.setDirection(y.REVERSE),
        this.animate()
      );
  }
  swap() {
    return this.reverseState(), this.animate();
  }
}
class A extends S {
  constructor(e) {
    super(), (this._animation = this.createAnimation(e));
  }
  createAnimation(e) {
    return a
      .timeline({
        targets: e.children,
        autoplay: !1,
        easing: "easeOutSine",
        duration: 250,
      })
      .add({
        opacity: (e, t) => {
          if (1 === t) return [1, 0];
        },
        translateY: (e, t) => (1 === t ? 0 : t ? -8 : 8),
      })
      .add({ rotateZ: (e, t) => (1 === t ? 0 : t ? -45 : 45) });
  }
}
var L = e({
  setup() {
    const e = r(null);
    let t = new E();
    return (
      l(() => {
        t.animation = new A(e.value);
      }),
      { button: e, animationController: t }
    );
  },
});
const O = { class: "cursor-pointer mr-4", ref: "button" },
  U = o("div", { class: "h-1 w-7 mb-1 rounded bg-current" }, null, -1),
  $ = o("div", { class: "h-1 w-7 mb-1 rounded bg-current" }, null, -1),
  H = o("div", { class: "h-1 w-7 rounded bg-current" }, null, -1);
L.render = function (e, n, r, a, l, o) {
  return s(), t("div", O, [U, $, H], 512);
};
class R extends S {
  constructor(e) {
    super(), (this._animation = this.createAnimation(e));
  }
  createAnimation(e) {
    return a
      .timeline({ easing: "easeOutSine", autoplay: !1 })
      .add({
        targets: e,
        opacity: [0, 1],
        translateX: ["-100vw", 0],
        duration: 150,
      })
      .add({
        targets: e.children,
        opacity: [0, 1],
        duration: 500,
        translateX: ["-100vw", 0],
        scale: [2, 1],
        delay: a.stagger(100),
      });
  }
}
var j = e({
  emits: ["click-outside"],
  setup(e, { emit: t }) {
    const n = r(null);
    let s = new E(),
      a = !0;
    l(() => {
      s.animation = new R(n.value);
    });
    const o = (e) =>
        (function (e, t) {
          const n =
            -1 !==
            Array.from(t.children).findIndex(
              (t) => t === e || t === e.parentElement
            );
          return !(e === t || n);
        })(e, n.value) && !a,
      c = (e) => {
        e.preventDefault();
        const n = e.target;
        o(n) && t("click-outside"), (a = !1);
      };
    return (
      i(
        () => s.state,
        () => {
          (a = !0),
            s.isOpened()
              ? document.addEventListener("click", c)
              : document.removeEventListener("click", c);
        }
      ),
      { animationController: s, navMenu: n, closeOnClickOutside: c }
    );
  },
});
const B = {
  class:
    "fixed top-0 left-0 flex flex-col items-start w-3/4 h-screen gap-6 py-5 pt-24 bg-black",
  ref: "navMenu",
};
j.render = function (e, r, a, l, o, i) {
  return s(), t("nav", B, [n(e.$slots, "default")], 512);
};
var T = e({
  components: { TheButtonSandwich: L, TheFloatMenu: j },
  setup() {
    const e = r(),
      t = r(),
      n = c();
    i(
      () => n.currentRoute.value.path,
      () => {
        s();
      }
    );
    const s = () => {
      var n, s;
      null == (n = e.value) || n.animationController.close(),
        null == (s = t.value) || s.animationController.close();
    };
    return {
      navMenu: e,
      buttonSandwich: t,
      swapState: async () => {
        var n, s;
        null == (n = e.value) || n.animationController.swap(),
          null == (s = t.value) || s.animationController.swap();
      },
      close: s,
    };
  },
});
const D = { class: "flex items-center justify-center text-white" };
T.render = function (e, r, a, l, i, c) {
  const h = u("the-button-sandwich"),
    v = u("the-float-menu");
  return (
    s(),
    t("nav", D, [
      o(
        h,
        {
          class: "text-white",
          ref: "buttonSandwich",
          onClick: d(e.swapState, ["prevent"]),
        },
        null,
        8,
        ["onClick"]
      ),
      o(
        v,
        { ref: "navMenu", onClickOutside: e.close },
        { default: m(() => [n(e.$slots, "default")]), _: 3 },
        8,
        ["onClickOutside"]
      ),
    ])
  );
};
var I = e({
  components: { TheHeaderNav: _, TheHeaderNavMobile: T },
  setup() {
    const e = r(null);
    return { mobile: r(!0), header: e, close: close };
  },
});
const N = {
  ref: "header",
  class:
    "\n      fixed\n      z-50\n      flex\n      justify-between\n      w-full\n      h-10\n      px-3\n      py-2\n      lg:h-16\n      lg:px-5\n      lg:py-3\n      bg-primary\n    ",
};
I.render = function (e, r, a, l, o, i) {
  const c = u("the-header-nav-mobile"),
    d = u("the-header-nav");
  return (
    s(),
    t(
      "header",
      N,
      [
        n(e.$slots, "logo"),
        e.mobile
          ? (s(),
            t(
              c,
              { key: 0 },
              { default: m(() => [n(e.$slots, "navMobile")]), _: 3 }
            ))
          : (s(),
            t(d, { key: 1 }, { default: m(() => [n(e.$slots, "nav")]), _: 3 })),
      ],
      512
    )
  );
};
var V = e({
  props: {
    to: { type: Object, required: !0 },
    active: { type: String, default: "" },
    mobileActive: { type: String, default: "" },
  },
  setup(e) {
    const t = v(),
      n = () => t.name === e.to.name;
    return {
      activeClasses: h(() => [
        n() && e.active,
        n() && window.visualViewport.width < 1e3 && e.mobileActive,
      ]),
    };
  },
});
V.render = function (e, r, a, l, o, i) {
  const c = u("router-link");
  return (
    s(),
    t(
      c,
      { to: e.to, class: ["duration-450 hover:duration-150", e.activeClasses] },
      { default: m(() => [n(e.$slots, "default")]), _: 3 },
      8,
      ["to", "class"]
    )
  );
};
const P = {},
  F = { xmlns: "http://www.w3.org/2000/svg", viewBox: "0 0 24 24" },
  q = o(
    "path",
    {
      d: "M12 5.69l5 4.5V18h-2v-6H9v6H7v-7.81l5-4.5M12 3L2 12h3v8h6v-6h2v6h6v-8h3L12 3z",
    },
    null,
    -1
  );
P.render = function (e, n) {
  return s(), t("svg", F, [q]);
};
const X = {},
  Q = { xmlns: "http://www.w3.org/2000/svg", viewBox: "0 0 24 24" },
  Y = o(
    "path",
    {
      d: "M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zM7.07 18.28c.43-.9 3.05-1.78 4.93-1.78s4.51.88 4.93 1.78C15.57 19.36 13.86 20 12 20s-3.57-.64-4.93-1.72zm11.29-1.45c-1.43-1.74-4.9-2.33-6.36-2.33s-4.93.59-6.36 2.33C4.62 15.49 4 13.82 4 12c0-4.41 3.59-8 8-8s8 3.59 8 8c0 1.82-.62 3.49-1.64 4.83zM12 6c-1.94 0-3.5 1.56-3.5 3.5S10.06 13 12 13s3.5-1.56 3.5-3.5S13.94 6 12 6zm0 5c-.83 0-1.5-.67-1.5-1.5S11.17 8 12 8s1.5.67 1.5 1.5S12.83 11 12 11z",
    },
    null,
    -1
  );
X.render = function (e, n) {
  return s(), t("svg", Q, [Y]);
};
const Z = {},
  G = { xmlns: "http://www.w3.org/2000/svg", viewBox: "0 0 24 24" },
  J = o(
    "path",
    {
      d: "M4 10.5c-.83 0-1.5.67-1.5 1.5s.67 1.5 1.5 1.5 1.5-.67 1.5-1.5-.67-1.5-1.5-1.5zm0-6c-.83 0-1.5.67-1.5 1.5S3.17 7.5 4 7.5 5.5 6.83 5.5 6 4.83 4.5 4 4.5zm0 12c-.83 0-1.5.68-1.5 1.5s.68 1.5 1.5 1.5 1.5-.68 1.5-1.5-.67-1.5-1.5-1.5zM7 19h14v-2H7v2zm0-6h14v-2H7v2zm0-8v2h14V5H7z",
    },
    null,
    -1
  );
Z.render = function (e, n) {
  return s(), t("svg", G, [J]);
};
var K = e({
  components: {
    TheButtonLinkInternal: V,
    HomeIcon: P,
    UserIcon: X,
    ListIcon: Z,
  },
});
const W = o("p", null, "Home", -1),
  ee = o("p", null, "Adicionar usuário", -1),
  te = o("p", null, "Ver usuários cadastrados", -1);
K.render = function (e, n, r, a, l, i) {
  const c = u("home-icon"),
    d = u("the-button-link-internal"),
    h = u("user-icon"),
    v = u("list-icon");
  return (
    s(),
    t(
      p,
      null,
      [
        o(
          d,
          {
            class: "grid grid-cols-link gap-2 btn-lg text-white font-bold",
            to: { name: "Home" },
          },
          { default: m(() => [o(c, { class: "text-white" }), W]), _: 1 }
        ),
        o(
          d,
          {
            class: "grid grid-cols-link gap-2 btn-lg text-white font-bold",
            to: { name: "AddUser" },
          },
          { default: m(() => [o(h, { class: "text-white" }), ee]), _: 1 }
        ),
        o(
          d,
          {
            class: "grid grid-cols-link gap-2 btn-lg text-white font-bold",
            to: { name: "AllUsers" },
          },
          { default: m(() => [o(v, { class: "text-white" }), te]), _: 1 }
        ),
      ],
      64
    )
  );
};
const ne = {},
  se = {
    xmlns: "http://www.w3.org/2000/svg",
    "xml:space": "preserve",
    version: "1.1",
    style: {
      "shape-rendering": "geometricPrecision",
      "text-rendering": "geometricPrecision",
      "image-rendering": "optimizeQuality",
      "fill-rule": "evenodd",
      "clip-rule": "evenodd",
    },
    viewBox: "0 0 4166.97 1199.4",
    "xmlns:xlink": "http://www.w3.org/1999/xlink",
  },
  re = o(
    "path",
    {
      d: "M3843.7 937.37l190.84 -549.81 0.75 550.92 130.85 1.17 0.83 -682.2 -214.32 -0.57 -169.91 510.03 -170.99 -509.84 -213.28 -0.17 0.55 682.84 130.53 -4.43 1.6 -539.08 184.31 542.52 128.24 -1.37zm-1062.48 -571.66l189.6 -0.62 7.29 570.46 136.11 3.92 0.61 -573.88 192.85 -1.25 4.17 -102.85 -531.45 -4.12 0.81 108.33zm-574.56 565.27l-6.33 -109.56c-79.62,6.38 -269.61,75.48 -353.89,-93.39 -107.7,-215.79 71.21,-463.43 346.46,-340.55l11.57 -115.11c-571.36,-159.55 -658.33,465.48 -378.21,626.17 132.6,76.07 232.56,48.81 380.4,32.44zm226.05 -357.48l0.5 -208.78c87.96,0 176.88,8.56 178.05,105.27 1.19,98.5 -91.25,103.42 -178.54,103.51zm-137.3 364.92l137.3 0.27 0.63 -258.69c189.57,0 369.32,-48.93 314.26,-273.49 -41.94,-171.05 -278.8,-153.36 -453.03,-148.06l0.85 679.97z",
    },
    null,
    -1
  ),
  ae = o(
    "path",
    {
      d: "M599.21 1199.4l335.05 0 -589.87 -599.7 331.88 -342.48 -335.16 0.39 -341.1 342.09 599.21 599.7zm175.1 -1199.4l-344.39 0 143.93 151.39 449.51 448.31 -332.95 334.12 333.89 0.05 344.01 -334.16 -594 -599.7z",
    },
    null,
    -1
  );
ne.render = function (e, n) {
  return s(), t("svg", se, [re, ae]);
};
var le = e({ components: { TheHeader: I, ENavMobile: K, CptmLogo: ne } });
le.render = function (e, n, r, a, l, i) {
  const c = u("cptm-logo"),
    d = u("e-nav-mobile"),
    h = u("the-header");
  return (
    s(),
    t(h, null, {
      logo: m(() => [o(c, { class: "w-28 text-white" })]),
      navMobile: m(() => [o(d)]),
      _: 1,
    })
  );
};
var oe = e({ components: { EHeader: le }, name: "App" });
const ie = { class: "w-full overflow-hidden" };
oe.render = function (e, n, r, a, l, i) {
  const c = u("e-header"),
    d = u("router-view");
  return (
    s(),
    t("div", ie, [
      o(c),
      o(d, null, {
        default: m(({ Component: e }) => [
          o(
            w,
            { name: "component", mode: "out-in" },
            { default: m(() => [(s(), t(f(e)))]), _: 2 },
            1024
          ),
        ]),
        _: 1,
      }),
    ])
  );
};
var ce = e({});
const ue = o("div", { class: "pt-10" }, "Adicionar usuário", -1);
ce.render = function (e, n, r, a, l, o) {
  const i = u("the-main");
  return s(), t(i, null, { default: m(() => [ue]), _: 1 });
};
var de = e({});
const me = o("div", { class: "pt-10" }, "Ver usuários", -1);
de.render = function (e, n, r, a, l, o) {
  const i = u("the-main");
  return s(), t(i, null, { default: m(() => [me]), _: 1 });
};
var he = e({ props: { to: { type: String, required: !0 } } });
he.render = function (e, r, a, l, o, i) {
  const c = u("router-link");
  return (
    s(),
    t(
      c,
      {
        class:
          "\n      grid grid-rows-card\n      gap-4\n      p-10\n      rounded-md\n      bg-secondary\n      w-full\n      h-45vh\n      cursor-pointer\n      shadow-md\n    ",
        to: { name: e.to },
      },
      { default: m(() => [n(e.$slots, "icon"), n(e.$slots, "name")]), _: 3 },
      8,
      ["to"]
    )
  );
};
var ve = e({});
const pe = { class: "min-h-screen" };
ve.render = function (e, r, a, l, o, i) {
  return s(), t("main", pe, [n(e.$slots, "default")]);
};
const we = {},
  fe = {
    xmlns: "http://www.w3.org/2000/svg",
    viewBox: "0 0 2511.9 2207.73",
    "xmlns:xlink": "http://www.w3.org/1999/xlink",
  },
  ge = o(
    "path",
    {
      d: "M294.36 858.56l0 49.06c0,107.44 64.08,224.24 118.93,278.46 65.61,64.86 167.72,133.65 263.74,133.65 -1.03,0.65 -2.71,0.23 -3.24,1.66l-137.38 29.43c-126.42,36.39 -232.98,100.17 -324.24,190.9 -16.62,16.53 -23.96,28.15 -39.23,44.17 -94.41,98.97 -172.94,302.02 -172.94,435.41l0 186.43 1471.82 0 0 -176.62c0,-312.11 -240.71,-597.12 -531.87,-679.93 -63.84,-18.16 -104.76,-18.55 -127.76,-24.32l-17.4 -7.13c96.42,0 198.07,-68.73 263.74,-133.65 54.85,-54.22 118.93,-171.02 118.93,-278.46 0,-138.86 -30.38,-238.89 -128.78,-337.3 -216.67,-216.67 -579.3,-151.8 -711.93,119.66 -19.81,40.54 -42.38,110.88 -42.38,168.58zm1040.09 -441.55l0 49.06c0,107.44 64.08,224.24 118.93,278.46 65.61,64.86 167.72,133.65 263.74,133.65l-21.49 7.94c-62.73,12.83 -89.79,6.98 -174.81,41.06 -31.24,12.52 -54.79,22.69 -85,37.65 -16.71,8.27 -23.97,12.19 -37.6,21.27 -61.25,40.82 -118.04,76.46 -170.26,138.83 -24.79,29.61 -89.97,109.8 -99.57,145.74 26.09,13.81 47.48,24.02 72.65,40.19l102.52 74.1c73.13,61.85 137.17,137.57 186.95,220.25 27.08,44.98 62.34,108.74 74.52,160.98l946.87 0 0 -159.08c-2.4,-98.56 -14.47,-186.31 -65.39,-295.58 -17.3,-37.14 -40.17,-78.04 -62.86,-108.85 -12.68,-17.21 -23.06,-34.96 -37.4,-50.91 -75.22,-83.66 -133.43,-138.6 -243.17,-193.47 -74.25,-37.13 -151,-59.83 -233.76,-70.42l-34.46 -9.7c46.73,0 118.98,-27.63 150.05,-46.2 57.05,-34.08 67.5,-41.79 113.7,-87.45 54.85,-54.22 118.93,-171.02 118.93,-278.46 0,-87.33 -3.54,-133.54 -40.85,-214.27 -63.48,-137.36 -221.33,-251.81 -376.17,-251.81 -138.63,0 -240.56,32.05 -337.3,128.78 -20.28,20.28 -30.4,31.15 -47.39,55.64 -44.59,64.28 -81.39,146.32 -81.39,232.59z",
    },
    null,
    -1
  );
we.render = function (e, n) {
  return s(), t("svg", fe, [ge]);
};
const xe = {},
  be = {
    xmlns: "http://www.w3.org/2000/svg",
    viewBox: "0 0 1340.5 1485.58",
    "xmlns:xlink": "http://www.w3.org/1999/xlink",
  },
  _e = o(
    "path",
    {
      d: "M43.52 43.52l319.17 0 0 319.17 -319.17 0 0 -319.17zm441.03 107.36c0,14.14 3.77,23.21 20.31,23.21l139.27 0c23.13,0 23.13,-43.52 0,-43.52l-139.27 0c-13.55,0 -20.31,6.76 -20.31,20.31zm0 1073.56c0,19.48 6.35,29.02 31.92,29.02l113.16 0c44.62,0 38.95,-46.42 8.71,-46.42l-130.57 0c-10.6,0 -23.21,8.01 -23.21,17.41zm0 -542.58c0,12.55 -2.07,29.02 29.02,29.02l118.96 0c41.1,0 35.91,-46.42 5.8,-46.42l-130.57 0c-10.6,0 -23.21,8.01 -23.21,17.41zm-319.17 -562.89c0,19.82 5.87,28.43 31.92,29.02l0 127.67c0,46.45 46.42,33.25 46.42,11.61l0 -162.48c0,-28.73 -29.94,-23.21 -55.13,-23.21 -10.6,0 -23.21,8.01 -23.21,17.41zm319.17 139.27c0,15.15 9.97,23.21 17.41,23.21l275.64 0c32.62,0 27.24,-43.52 5.8,-43.52l-278.55 0c-13.55,0 -20.31,6.77 -20.31,20.31zm0 1076.46c0,14.14 3.77,23.21 20.31,23.21l278.55 0c24.23,0 21.25,-43.52 0,-43.52l-281.45 0c-4.52,0 -9.48,3.64 -11.69,5.72 -4.29,4.04 -5.72,6.3 -5.72,14.59zm0 -545.48c0,18.46 4.05,29.02 29.02,29.02l255.33 0c45.81,0 37.68,-46.42 8.71,-46.42l-269.84 0c-10.6,0 -23.21,8.01 -23.21,17.41zm-327.87 -72.54c43.21,0 6.31,-41.07 58.03,-29.02 5.15,22.1 3.67,25.12 -18.23,54.31l-52.77 75.1c-10.12,39.97 58.72,30.17 97.11,30.17 17.3,0 29.02,-5.92 29.02,-23.21 0,-29 -29.8,-23.21 -58.03,-23.21 4.21,-15.76 52.23,-63.24 52.23,-95.75 0,-60.96 -73.98,-94.22 -116.61,-38.27 -21.51,28.23 -1.95,49.88 9.26,49.88zm-14.51 516.47c0,18.09 17.01,25.48 30.9,19.56 18.27,-7.78 8.72,-31.17 30.04,-31.17 17.03,0 33.08,28.43 2.14,36.95 -4.83,1.33 -22.45,0.34 -22.45,23.98 0,25.37 19.4,16.28 32.55,25.56 14.55,10.27 5.17,32.47 -9.34,32.47 -35.39,0 -4.3,-40.62 -46.42,-40.62 -13.5,0 -25.27,20.59 -14.63,46.53 6.45,15.71 21.43,30.71 39.13,36.35 61.48,19.6 108.67,-53.89 68.35,-100.28 18.31,-27.35 22.05,-60.11 -2.04,-85.01 -40.78,-42.15 -108.22,-11.72 -108.22,35.68zm-98.65 -110.26l319.17 0 0 319.17 -319.17 0 0 -319.17zm-43.52 -8.71l0 339.48c0,14.75 17.17,31.92 31.92,31.92l342.38 0c16.21,0 28.66,-12.15 31.92,-26.11l832.56 -0.18c19.29,-2.6 24.46,-22.25 32.1,-37.54 13.9,-27.82 69.64,-125.19 69.64,-142.17 0,-9.58 -64.64,-130.64 -71.61,-143.1 -12.54,-22.43 -13.85,-33.89 -38.65,-33.89l-351.08 0c-29.43,0 -39.09,43.34 -0.03,46.45l345.31 -0.03 34.82 66.73c43.66,87.8 42.62,45.28 -0.97,132.51 -6.54,13.08 -10.75,21.8 -18.12,34.11 -4.98,8.33 -11.46,30.69 -21.53,30.69l-809.52 0 0 -264.04 362.69 0c12.94,0 26.11,-11.75 26.11,-23.21 0,-30.68 -47.5,-23.21 -78.34,-23.21l-313.36 0c-3.25,-13.97 -15.7,-26.11 -31.92,-26.11l-339.48 0c-9.24,0 -18.7,5.71 -24.13,10.68 -7.47,6.83 -10.68,13.84 -10.68,27.04zm43.52 -530.98l319.17 0 0 319.17 -319.17 0 0 -319.17zm1180.92 293.05l-815.33 0 0 -266.94 815.33 0c5.11,19.13 66.73,120.84 66.73,133.47 0,12.57 -61,111.99 -66.73,133.47zm-1224.44 -304.66l0 342.38c0,23.31 21.95,34.82 46.42,34.82l319.17 0c19.9,0 39.1,-10.74 40.62,-29.02 19.35,0 803.26,3.17 832.83,0.09 19.83,-2.06 24.37,-23.6 31.79,-37.85 4.47,-8.59 7.8,-12.6 12.65,-22.16 74.87,-147.51 73.16,-87.7 0.96,-232.2 -4.84,-9.68 -7.76,-13.16 -12.62,-22.2 -30.29,-56.26 -7.86,-39.67 -169.24,-39.67 -231.15,0 -462.31,0 -693.46,0 -8.28,-15.65 -19.71,-29.02 -43.52,-29.02l-319.17 0c-24.48,0 -46.42,11.51 -46.42,34.82zm0 -539.68l0 342.38c0,17.43 17.39,34.82 34.82,34.82l339.48 0c16.21,0 28.66,-12.15 31.92,-26.11 10.65,0 810.22,2.93 832.2,-0.54 11.27,-1.78 14.73,-4.83 19.99,-14.82 4.96,-9.42 8.13,-13.94 12.5,-22.32 7.8,-14.98 14.87,-31.82 23.3,-46.33 8.71,-14.99 15.65,-28.67 24.04,-45.59 4.69,-9.46 7.01,-13.75 12.33,-22.49 21.71,-35.63 3.82,-44.2 -35.49,-121.66l-24.22 -45.41c-7.4,-14.81 -13.78,-37.72 -34.82,-37.72l-359.79 0c-15.92,0 -23.21,12.71 -23.21,23.21 0,29.15 45.43,23.21 75.44,23.21l295.96 0c4.61,8.71 68.24,126.07 67.76,132.91 -0.09,1.3 -29.99,58.58 -33.88,66.36 -4.96,9.93 -30.55,64.77 -39.69,64.77l-809.52 0 0 -264.04 351.08 0c18.86,0 34.32,-3.81 36.75,-21.33 2.84,-20.48 -15.76,-25.38 -33.71,-25.24l-357.03 0.14c-3.8,-16.31 -14.69,-26.11 -34.82,-26.11l-339.48 0c-14.75,0 -31.92,17.17 -31.92,31.92z",
    },
    null,
    -1
  );
xe.render = function (e, n) {
  return s(), t("svg", be, [_e]);
};
var ze = e({
  components: { ECard: he, TheMain: ve, UserCardIcon: we, ListCardIcon: xe },
});
const ye = o(
    "p",
    { class: "flex justify-center items-center text-white text-2xl font-bold" },
    " Adicionar usuário ",
    -1
  ),
  Me = o(
    "p",
    { class: "flex justify-center items-center text-white text-2xl font-bold" },
    " Listar usuários ",
    -1
  );
ze.render = function (e, n, r, a, l, i) {
  const c = u("user-card-icon"),
    d = u("e-card"),
    h = u("list-card-icon"),
    v = u("the-main");
  return (
    s(),
    t(
      v,
      { class: "flex flex-col gap-4 pt-14 lg:pt-20 pb-4 px-4" },
      {
        default: m(() => [
          o(
            d,
            { to: "AddUser" },
            {
              icon: m(() => [o(c, { class: "text-white" })]),
              name: m(() => [ye]),
              _: 1,
            }
          ),
          o(
            d,
            { to: "AllUsers" },
            {
              icon: m(() => [o(h, { class: "text-white" })]),
              name: m(() => [Me]),
              _: 1,
            }
          ),
        ]),
        _: 1,
      }
    )
  );
};
const ke = [
    { path: "/", name: "Home", component: ze },
    { path: "/todos-usuarios", name: "AllUsers", component: de, props: !0 },
    { path: "/adicionar-usuario", name: "AddUser", component: ce },
  ],
  Ce = g({
    history: x("/"),
    routes: ke,
    scrollBehavior: () => window.scrollTo({ top: 0, behavior: "smooth" }),
  });
b(oe).use(Ce).mount("#app");