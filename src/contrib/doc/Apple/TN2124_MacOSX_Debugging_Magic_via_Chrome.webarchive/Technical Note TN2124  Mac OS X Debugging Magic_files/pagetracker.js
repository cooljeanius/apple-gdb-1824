/* Copyright © 2010 Apple Inc.  All rights reserved. */
var PageTracker = {
  server: "https://devstats.apple.com/webstats/pagetracker.php",
  imgObj: null,
  count: 0,
  encodeURIComponent: function (a) {
    if (encodeURIComponent) {
      return encodeURIComponent(a);
    } else {
      return window.escape(a);
    }
  },
  logEvent: function (a, g) {
    if (a && g) {
      if (this.imgObj === null) {
        this.imgObj = new Image(1, 1);
      }
      if (this.imgObj) {
        var e = [];
        e.push("_event=" + this.encodeURIComponent(a));
        this.count += 1;
        e.push("_c=" + this.encodeURIComponent(this.count));
        e.push("_pt=" + this.encodeURIComponent(document.title));
        var f = window.location.href;
        var b = f.indexOf("#");
        if (b > 0 && b + 1 < f.length) {
          e.push("_anc=" + this.encodeURIComponent(f.substr(b + 1)));
        }
        var c;
        for (c in g) {
          if (g[c] !== null) {
            e.push(c + "=" + this.encodeURIComponent(g[c]));
          }
        }
        var d = e.join("&");
        this.imgObj.src = this.server + "?" + d;
      }
    }
  },
  logPageLoad: function () {
    var a = {};
    if (document.referrer) {
      a.referrer = document.referrer;
    }
    var b = document.getElementById("identifier");
    if (b) {
      a.identifier = b.content;
    }
    this.logEvent("pageLoad", a);
  },
};
