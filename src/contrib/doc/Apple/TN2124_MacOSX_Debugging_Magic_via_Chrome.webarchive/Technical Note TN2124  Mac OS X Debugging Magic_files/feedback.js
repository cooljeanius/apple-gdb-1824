var $_ = function (a) {
    var c = a;
    $_.hasOwnProperty("namespace") &&
      (c = $_.namespace + " " + a.replace("#", "#modal_"));
    a = document.querySelectorAll(c);
    return 1 === a.length ? a[0] : [].slice.call(a);
  },
  $_animationEndEvent = function () {
    var a = document.body.style;
    return "webkitTransition" in a
      ? "webkitTransitionEnd"
      : "mozTransition" in a
        ? "transitionend"
        : "oTransition" in a
          ? "otransitionend"
          : "transitionend";
  };
HTMLElement.prototype.forEach = function (a) {
  a(this);
};
function DPFeedbackForm(a) {
  this.validator = new DPFeedbackValidator();
  this.data = {
    docURL: a.path(),
    docVersion: a.metaTag("document-version"),
    build_uid: a.metaTag("build"),
    project_appleref: a.metaTag("identifier"),
    lang: a.language(),
    media: a.userAgent(),
  };
  this.requiredFields = [$_(".radio"), $_("#comment")];
}
DPFeedbackForm.prototype = {
  get: function (a) {
    return a ? this.data[a] || null : this.data;
  },
  set: function (a, c) {
    "helped" === a && this.validator.pass($_("#helpful_title"));
    if ("problem" === a) {
      var b = $_("#comment");
      if (this.validator.isValid(b)) this.validator.pass(b);
      else {
        this.validator.fail(b);
        this.remove(a);
        return;
      }
    }
    c ? (this.data[a] = c) : this.remove(a);
  },
  remove: function (a) {
    this.get(a) && delete this.data[a];
  },
  remainingFields: function () {
    for (var a = 0, c = [], b; a < this.requiredFields.length; )
      (b = this.requiredFields[a]),
        (b =
          b instanceof Array
            ? b[0].getAttribute("name")
            : b.getAttribute("name")),
        this.get(b) || c.push(this.requiredFields[a]),
        a++;
    return c;
  },
  validator: function () {
    return this.validator;
  },
};
function DPFeedbackMetadata() {}
DPFeedbackMetadata.prototype = {
  language: function () {
    return window.navigator.language;
  },
  metaTag: function (a) {
    return document.getElementById(a).getAttribute("content");
  },
  path: function () {
    return window.location.href;
  },
  userAgent: function () {
    var a = window.navigator.userAgent;
    return /Xcode/.test(a) ? "docset" : /iPad/.test(a) ? "ipad" : "web";
  },
};
function DPFeedbackSubmitter() {
  this.url = window.location.origin + "/feedback/survey.php";
  this.ok = 200;
  this.TIMEOUT = 1e4;
  this.SPLASH = 4e3;
  this.INTERVAL = 100;
  this.abort = function (a) {
    a.abort();
    $_("#sending").classList.add("hidden");
    $_("#error").classList.remove("hidden");
    $_("#error h2").focus();
  };
}
DPFeedbackSubmitter.prototype = {
  sendData: function (a, c) {
    var b = this.abort,
      e = this.ok,
      k = this.TIMEOUT,
      l = this.INTERVAL,
      d = new XMLHttpRequest();
    d.open("POST", this.url, !0);
    d.setRequestHeader("Content-type", "application/x-www-form-urlencoded", !0);
    d.withCredentials = !0;
    var g = 0,
      m = setInterval(function () {
        1 === g
          ? ($_("#sending").classList.remove("hidden"),
            $_("#sending h2").focus())
          : g === k / l && b(d);
        g++;
      }, this.INTERVAL);
    d.onreadystatechange = function () {
      4 === d.readyState &&
        (clearTimeout(m), d.status === e ? c(d.responseText) : b(d));
    };
    var f = [],
      h;
    for (h in a) f.push(encodeURIComponent(h) + "=" + encodeURIComponent(a[h]));
    f = f.join("&");
    d.send(f);
    return d;
  },
  processResponse: function (a) {
    "EventManager" in window &&
      window.EventManager.inst().emit({ name: "InternalFeedbackSubmitted" });
    $_("#success").classList.remove("hidden");
    $_("#success h2").focus();
    $_("#feedback").setAttribute("aria-hidden", "true");
  },
};
function DPFeedbackValidator() {}
DPFeedbackValidator.prototype = {
  isValid: function (a) {
    switch (a.getAttribute("name")) {
      case "problem":
        return !this.isEmpty(a);
    }
  },
  isEmpty: function (a) {
    return 0 === a.value.trim().length;
  },
  pass: function (a) {
    this.hideAsterisk(a);
    void 0 === $_(".invalid").length &&
      $_(".fineprint").classList.remove("invalid");
  },
  showAsterisk: function (a) {
    a.classList.add("invalid");
    $_(".fineprint").classList.add("invalid");
    $_("#" + a.dataset.asterisk).classList.add("invalid");
  },
  hideAsterisk: function (a) {
    a.classList.remove("invalid");
    $_("#" + a.dataset.asterisk).classList.remove("invalid");
    a.removeAttribute("aria-invalid");
  },
  fail: function (a) {
    a.setAttribute("aria-invalid", "true");
    this.showAsterisk(a);
    var c = this;
    $_(".invalid").forEach(function (a) {
      a.addEventListener("keyup", function () {
        c.isEmpty(a) || c.pass(a);
      });
    });
  },
};
function DPFeedbackWindow() {
  this.data = null;
  this.metadata = {};
  this.req = this.originalLocation = this.activeElement = null;
  $_.hasOwnProperty("namespace") ||
    ((this.root = $_("#blackout")), (this.button = $_("#leave_feedback")));
  this.modal = $_("#modal");
  this.form = $_("#feedback");
  this.state = "closed";
  this.MODALHEIGHT = 436;
  this.reset = function () {
    this.form.reset();
    this.data = new DPFeedbackForm(this.metadata);
    this.activeElement = null;
    $_("#success").classList.add("hidden");
    $_("#sending").classList.add("hidden");
    $_("#error").classList.add("hidden");
  };
}
DPFeedbackWindow.prototype = {
  open: function () {
    this.state = "opening";
    this.form.setAttribute("aria-hidden", "false");
    this.data
      ? this.originalLocation !== this.metadata.metaTag("identifier") &&
        (this.reset(),
        (this.originalLocation = this.metadata.metaTag("identifier")))
      : ((this.metadata = new DPFeedbackMetadata()),
        (this.data = new DPFeedbackForm(this.metadata)),
        (this.originalLocation = this.metadata.metaTag("identifier")));
    $_.hasOwnProperty("namespace") ||
      ((this.modal.style.overflow = "visible"),
      this.root.classList.add("activated"),
      this.root.classList.add("fixed"),
      this.modal.classList.add("activated"),
      (this.modal.style.height = this.MODALHEIGHT + "px"),
      (this.modal.style.zIndex = 994));
  },
  cancel: function () {
    "sending" === this.state
      ? $_("#success").classList.contains("hidden")
        ? ((this.state = "closing"),
          this.req &&
            (this.req.abort(), $_("#sending").classList.add("hidden")),
          $_.hasOwnProperty("namespace") ||
            (this.root.classList.remove("activated"),
            this.modal.classList.remove("activated")))
        : $_.hasOwnProperty("namespace") ||
          (this.root.classList.remove("activated"),
          this.modal.classList.add("sent"))
      : ((this.state = "closing"),
        $_.hasOwnProperty("namespace") ||
          (this.modal.setAttribute("aria-hidden", "true"),
          this.button.focus(),
          this.root.classList.remove("activated"),
          this.modal.classList.remove("activated")));
    $_.hasOwnProperty("namespace") || (this.modal.style.overflow = "hidden");
  },
  send: function () {
    var a = this.data.remainingFields();
    if (0 === a.length) {
      this.state = "sending";
      var a = this.data.get(),
        c = new DPFeedbackSubmitter();
      this.req = c.sendData(a, c.processResponse);
    } else {
      var b = this.data.validator;
      a.forEach(function (a) {
        a instanceof Array && (a = $_("#helpful_title"));
        b.fail(a);
      });
      a[0] instanceof Array ? $_("#helpful_title").focus() : a[0].focus();
    }
  },
  animationDidFinish: function () {
    switch (this.state) {
      case "opening":
        this.modal.setAttribute("aria-hidden", "false");
        this.activeElement
          ? this.activeElement.focus()
          : $_("#helpful_title").focus();
        this.state = "opened";
        break;
      case "closing":
        this.modal.style.height = 0;
        this.root.classList.remove("fixed");
        $_("#error").classList.add("hidden");
        this.modal.setAttribute("aria-hidden", "true");
        this.button.focus();
        this.state = "closed";
        break;
      case "sending":
        (this.modal.style.overflow = "hidden"),
          (this.modal.style.height = 0),
          this.root.classList.remove("fixed"),
          this.modal.classList.remove("activated"),
          this.modal.classList.remove("sent"),
          this.modal.setAttribute("aria-hidden", "true"),
          this.button.focus(),
          this.reset(),
          (this.state = "sent");
    }
  },
  interpretKeystroke: function (a) {
    27 === a && this.modal.classList.contains("activated") && this.cancel();
  },
  rememberFocus: function (a) {
    this.activeElement = a;
  },
  valueDidChange: function (a) {
    var c = a.getAttribute("type"),
      b = a.value;
    "radio" === c
      ? (b = parseInt(b, 10))
      : "checkbox" === c && (b = a.checked ? 1 : 0);
    this.data.set(a.getAttribute("name"), b);
  },
  setMetadata: function (a) {
    for (var c in a) this.metadata[c] = a[c];
    this.data = new DPFeedbackForm(this.metadata);
  },
};
window.addEventListener("load", function () {
  if (
    $_("#modal") &&
    $_("#leave_feedback") &&
    $_("#blackout") &&
    $_("#closebox") &&
    $_("#feedback") &&
    $_("#submit") &&
    !/Xcode/.test(window.navigator.userAgent)
  ) {
    var a = $_("#StaticPage");
    if (a instanceof HTMLElement) {
      var c = a.querySelectorAll(
        "#contents #modal, #contents #leave_feedback, #contents #blackout",
      );
      Array.prototype.forEach.call(c, function (b) {
        b.parentNode.removeChild(b);
        a.appendChild(b);
      });
    }
    var b = new DPFeedbackWindow(),
      e = document.ontouchend ? "touchend" : "click";
    $_("#modal").addEventListener(
      $_animationEndEvent(),
      function () {
        b.animationDidFinish();
      },
      !1,
    );
    $_("#leave_feedback").addEventListener(
      e,
      function () {
        b.open();
      },
      !1,
    );
    $_("#blackout, #closebox").forEach(function (a) {
      a.addEventListener(
        e,
        function () {
          b.cancel();
        },
        !1,
      );
    });
    $_("#feedback input, #feedback textarea").forEach(function (a) {
      a.addEventListener(
        "change",
        function () {
          b.valueDidChange(this);
        },
        !1,
      );
      a.addEventListener(
        "focus",
        function () {
          b.rememberFocus(this);
        },
        !1,
      );
    });
    $_("#feedback").addEventListener(
      "submit",
      function (a) {
        a.preventDefault();
        b.send();
      },
      !1,
    );
    $_("#submit").addEventListener(
      e,
      function () {
        b.send();
      },
      !1,
    );
    document.addEventListener(
      "keyup",
      function (a) {
        b.interpretKeystroke(a.keyCode);
      },
      !1,
    );
  }
});
