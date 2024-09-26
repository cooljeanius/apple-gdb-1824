function Xcode() {
  var _self = this;
  _self.version = void 0;
}
function Querystring(qs) {
  var args, i, pair, name, value;
  if (
    ((this.params = {}),
    qs || (qs = location.search.substring(1, location.search.length)),
    0 === qs.length)
  )
    return null;
  for (args = qs.replace(/\+/g, " ").split("&"), i = 0; args.length > i; i++)
    (pair = args[i].split("=")),
      (name = decodeURIComponent(pair[0])),
      (value = 2 === pair.length ? decodeURIComponent(pair[1]) : name),
      (this.params[name] = value);
}
function targetIsInputField(target) {
  var inputFieldRegex = /(input|textarea)/;
  return target.tagName.toLowerCase().match(inputFieldRegex) ? !0 : !1;
}
function ContentFrame() {
  return this.pageDidLoad(), (this.cJump = ""), (this.jump = ""), this;
}
function selectJumpToItem(item) {
  document.getElementById("jumpTo").value = item;
}
function setupJumpTo() {
  function select() {
    var jumpHashValue = document.getElementById("jumpTo").value,
      jumpName = document.getElementById(jumpHashValue);
    if (null === jumpName) {
      var query = "[name='" + jumpHashValue + "']";
      jumpName = document.querySelector(query);
    }
    if (jumpName) {
      jumpName.scrollIntoView();
      var location_hash = "#" + jumpHashValue;
      if (window.location.hash !== location_hash) {
        var state_obj = {},
          article = document.getElementsByTagName("article")[0];
        (state_obj.page = article.innerHTML),
          (state_obj.title = document.title);
        var fullPathWithHash = window.location.pathname + "#" + jumpHashValue;
        history.pushState(state_obj, document.title, fullPathWithHash),
          (HistoryState.firstLoad = !1);
      }
    }
  }
  var menuItem,
    textForMenu,
    i,
    jumpTos,
    currentJump,
    spaces = "&nbsp;&nbsp;&nbsp;&nbsp;",
    indentedSymbols =
      /\btypeDef\b|\bconstantName\b|\bfunction\b|\bmethod_property\b|\bcallback\b|\bstruct\b|\bnotification\b|\bvariableName\b/;
  if (
    ((jumpTos = document.body.getElementsBySelector(".jump")),
    "Reference" === Book.type && jumpTos.length > 1)
  ) {
    for (
      document.getElementById("jumpTo").innerHTML = "",
        menuItem = document.createElement("option"),
        menuItem.innerHTML = "Jump To…",
        menuItem.value = "top",
        document.getElementById("jumpTo").insert({ bottom: menuItem }),
        i = 0;
      jumpTos.length > i;
      i++
    )
      if (
        ((currentJump = jumpTos[i]),
        (currentJump.id = "jumpTo_" + i),
        (menuItem = document.createElement("option")),
        (textForMenu = currentJump.textContent
          ? currentJump.textContent
          : currentJump.innerHTML),
        indentedSymbols.test(currentJump.className)
          ? ((menuItem.innerHTML = spaces + textForMenu),
            (menuItem.value = currentJump.id))
          : currentJump.hasClassName("instanceMethod")
            ? ((menuItem.innerHTML = spaces + "-&nbsp;" + textForMenu),
              (menuItem.value = currentJump.id))
            : currentJump.hasClassName("classMethod")
              ? ((menuItem.innerHTML = spaces + "+&nbsp;" + textForMenu),
                (menuItem.value = currentJump.id))
              : ((menuItem.innerHTML = textForMenu),
                (menuItem.value = currentJump.previous("a").name)),
        document.getElementById("jumpTo").insert({ bottom: menuItem }),
        window.document.location.hash === "#" + menuItem.value)
      ) {
        menuItem.selected = !0;
        var jumpName = document.getElementById(menuItem.value);
        jumpName && jumpName.scrollIntoView();
      }
    return (
      document.getElementById("jumpto_button").show(),
      document.getElementById("jumpTo").observe("change", select),
      document.getElementById("jumpTo")
    );
  }
  return (
    document.getElementById("jumpto_button").hide(),
    document.getElementById("jumpTo").stopObserving("change", select),
    null
  );
}
function ADCSearchController() {
  function closeMenu(event) {
    event.element().descendantOf("ssi_SearchMenu") ||
      ($("ssi_SearchButton").removeClassName("active"),
      $("ssi_SearchMenu").removeClassName("open"),
      document.stopObserving("click", closeMenu));
  }
  function openMenu(event) {
    event.stop(),
      $("ssi_SearchMenu").hasClassName("open")
        ? closeMenu(event)
        : ($("ssi_SearchButton").addClassName("active"),
          $("ssi_SearchMenu").addClassName("open"),
          $("ssi_SearchField").focus(),
          document.observe("click", closeMenu));
  }
  return $("ssi_SearchButton")
    ? ((this.toggleMenu = openMenu),
      $("ssi_SearchButton").observe("click", openMenu))
    : null;
}
function Pedia(pediaContent, book_resource_type) {
  if (
    ((pediaContent && book_resource_type) ||
      ((pediaContent = document.getElementById("pediaContents")),
      (book_resource_type = document.getElementById("book-resource-type"))),
    book_resource_type && "DevPedia" === book_resource_type.content)
  ) {
    this.createPediaLinksBox(pediaContent);
    for (
      var links = pediaContent.getElementsByTagName("a"), i = 0;
      links.length > i;
      i++
    )
      links[i].up(".pediaLink") || (links[i].target = "_top");
  } else
    $$(".pediaLink a").forEach(function (pediaLink) {
      pediaLink.setAttribute("data-href", pediaLink.getAttribute("href")),
        pediaLink.removeAttribute("href");
    }),
      document.body.observe("click", this.click.bindAsEventListener(this)),
      Event.observe(window, "resize", this.hide.bindAsEventListener(this)),
      (this.popover = document.getElementById("pediaWindow"));
  return this;
}
function Recipe() {
  var i;
  if ($("book-resource-type") && "Recipe" == $("book-resource-type").content) {
    var links = document.getElementsByTagName("a");
    for (i = 0; links.length > i; i++)
      links[i].up(".recipeLink")
        ? links[i].observe("click", this.click.bindAsEventListener(this))
        : (links[i].target = "_top");
  } else if (!Prototype.Browser.Xcode) {
    var recipeLinks = document.getElementsByClassName("recipeLink");
    for (i = 0; recipeLinks.length > i; i++)
      recipeLinks[i].observe("click", this.click.bindAsEventListener(this));
  }
  return this;
}
(Xcode.prototype = {
  init: function () {
    var _self = this,
      ua = navigator.userAgent;
    (_self.version = parseFloat(ua.slice(ua.indexOf("Xcode/") + 6, ua.length))),
      _self.addCSS("xcode4.css", "xcode4"),
      _self.version >= 5 && _self.addCSS("xcode5.css", "xcode5");
  },
  addCSS: function (filename, css_id) {
    var cssPath,
      head = document.getElementsByTagName("head")[0],
      linkTagHref = head.querySelector("link[rel=stylesheet]").href;
    cssPath = linkTagHref.substring(0, linkTagHref.lastIndexOf("/") + 1);
    var csslink = document.createElement("link");
    csslink.setAttribute("rel", "stylesheet"),
      csslink.setAttribute("type", "text/css"),
      csslink.setAttribute("href", cssPath + filename),
      csslink.setAttribute("id", css_id),
      head.appendChild(csslink);
  },
  Xcode4OrNewer: function () {
    var _self = this;
    return 1e3 > _self.version;
  },
}),
  window.addEventListener("DOMContentLoaded", function () {
    var ua = navigator.userAgent,
      isXcode = ua.indexOf("Xcode/") > -1;
    isXcode ? ((xcode = new Xcode()), xcode.init()) : (xcode = void 0);
  });
var COOKIE_EXPIRE_DATE = new Date();
COOKIE_EXPIRE_DATE.setFullYear(COOKIE_EXPIRE_DATE.getFullYear() + 1),
  (Prototype.Browser.Xcode = navigator.userAgent.indexOf("Xcode/") > -1),
  (Prototype.Browser.XcodeVersion = parseFloat(
    navigator.userAgent.slice(
      navigator.userAgent.indexOf("Xcode/") + 6,
      navigator.userAgent.length,
    ),
  )),
  (Prototype.Browser.Xcode4OrNewer = 1e3 > Prototype.Browser.XcodeVersion),
  (String.prototype.directoryForURL = function () {
    var url = this.split("#")[0];
    return url.replace(/[^\/]+\.[^\/]+$/, "");
  }),
  (String.prototype.isHTTPurl = function () {
    return this.match(/^https?:\/\//);
  });
var DevPubs = {
    init: function () {},
    generator: document.getElementById("generator")
      ? document.getElementById("generator").content
      : "Unknown",
    Window: function (id) {
      var box, header, contents, footer;
      (box = document.createElement("div")),
        (header = document.createElement("div")),
        (contents = document.createElement("div")),
        (footer = document.createElement("div")),
        box.setAttribute("id", id + "Window"),
        header.setAttribute("id", id + "Header"),
        contents.setAttribute("id", id + "Body"),
        footer.setAttribute("id", id + "Footer"),
        box.appendChild(header),
        box.appendChild(contents),
        box.appendChild(footer),
        (box.style.display = "none"),
        document.body.appendChild(box),
        (this.box = $(id + "Window")),
        (this.header = $(id + "Header")),
        (this.contents = $(id + "Body")),
        (this.footer = $(id + "Footer"));
    },
    constructNativeVideos: function () {
      Array.prototype.forEach.call(
        document.getElementsByTagName("video"),
        function (video) {
          new NativeVideo(video);
        },
      );
    },
  },
  Cookie = {
    set: function (parameters) {
      if (null !== localStorage && "undefined" != typeof localStorage) {
        try {
          localStorage.setItem(parameters.cName, parameters.cValue);
        } catch (e) {
          return e;
        }
        return parameters.cValue;
      }
      return (
        "" === parameters.cDomain &&
          window.location.domain &&
          (parameters.cDomain = window.location.domain),
        (document.cookie =
          parameters.cName +
          "=" +
          escape(parameters.cValue) +
          (parameters.cExpiry
            ? "; expires=" + parameters.cExpiry.toGMTString()
            : "") +
          (parameters.cDomain ? "; domain=" + parameters.cDomain : "") +
          (parameters.cPath
            ? "; path=" + parameters.cPath
            : "; path=" + window.location.pathname))
      );
    },
    get: function (cookie_name) {
      if (
        null !== localStorage &&
        "undefined" != typeof localStorage &&
        "null" !== localStorage.getItem(cookie_name)
      )
        return localStorage.getItem(cookie_name);
      var cookie_array, i, aCookie;
      for (
        cookie_array = document.cookie.split(";"), i = 0;
        cookie_array.length > i;
        i++
      ) {
        for (aCookie = cookie_array[i]; " " === aCookie.charAt(0); )
          aCookie = aCookie.substring(1, aCookie.length);
        if (0 === aCookie.indexOf(cookie_name + "="))
          return null !== localStorage && "undefined" != typeof localStorage
            ? (localStorage.setItem(
                cookie_name,
                aCookie.substring(cookie_name.length + 1, aCookie.length),
              ),
              Cookie.forceExpire(cookie_name),
              localStorage.getItem(cookie_name))
            : aCookie.substring(cookie_name.length + 1, aCookie.length);
      }
      return null;
    },
    expire: function (cookie_name) {
      return null !== localStorage && "undefined" != typeof localStorage
        ? localStorage.removeItem(cookie_name)
        : Cookie.forceExpire(cookie_name);
    },
    forceExpire: function (cookie_name) {
      var past = new Date();
      return (
        past.setUTCFullYear(2e3),
        this.set({
          cName: cookie_name,
          cValue: null,
          cExpiry: past,
          cPath: "/",
        })
      );
    },
  };
(Querystring.prototype.get = function (key, default_) {
  var value = this.params[key];
  return null !== value ? value : default_;
}),
  (Querystring.prototype.contains = function (key) {
    var value = this.params[key];
    return null !== value;
  }),
  (Prototype.Query = new Querystring()),
  (Prototype.HashQuery = new Querystring(location.hash.replace(/#/, "")));
var NativeVideo = function (video) {
  var _this = this,
    overlay = document.createElement("div");
  overlay.setAttribute("class", "playButtonOverlay");
  var container = video.parentNode;
  return (
    container.insertBefore(overlay, video),
    video.addEventListener("click", function (event) {
      _this.togglePlayback(event);
    }),
    video.addEventListener("touchend", function (event) {
      _this.togglePlayback(event);
    }),
    video.addEventListener("play", function () {
      _this.play();
    }),
    video.addEventListener("pause", function () {
      _this.pause();
    }),
    video.addEventListener("ended", function () {
      _this.rewind();
    }),
    video.addEventListener("keypress", function (e) {
      32 === e.keyCode && (e.preventDefault(), _this.togglePlayback(e));
    }),
    (this.NATIVE_CONTROLS_HEIGHT = 24),
    (this.video = { element: video, height: video.getAttribute("height") }),
    (this.overlay = container.querySelector(".playButtonOverlay")),
    this.video.element.setAttribute("controls", "controls"),
    this.video.element
  );
};
(NativeVideo.prototype = {
  togglePlayback: function (e) {
    (e && e.offsetY > this.video.height - this.NATIVE_CONTROLS_HEIGHT) ||
      (this.isPaused() ? this.play() : this.pause());
  },
  play: function () {
    var vid = this.video.element;
    this.overlay.classList.add("hide"), vid.play(), vid.focus();
  },
  pause: function () {
    var vid = this.video.element,
      overlay = this.overlay;
    vid.pause(), vid.focus(), overlay.classList.remove("hide");
  },
  isPaused: function () {
    return this.video.element.paused;
  },
  rewind: function () {
    this.pause(), (this.video.element.currentTime = 0);
  },
}),
  window.addEventListener("load", function () {
    DevPubs.init(),
      Tasks !== void 0 && Tasks.init(),
      DevPubs.constructNativeVideos();
  }),
  Prototype.Browser.Xcode &&
    Prototype.Browser.Xcode4OrNewer &&
    (document.body.className += " Xcode");
var contentFrame,
  Book = {},
  RefLib = {},
  Paths = {},
  jazzDocs = ["MobileHIG", "UIKitUICatalog", "TransitionGuide"];
(String.prototype.isHTTPurl = function () {
  return this.match(/^https?:\/\//);
}),
  (String.prototype.cleanUpURL = function () {
    var newURL,
      hash = "",
      url = this;
    if (
      (this.match("#") &&
        ((hash = "#" + this.split("#")[1]), (url = this.split("#")[0])),
      (url = url.replace(/\/\.\/|([^:])\/\//g, "$1/")),
      "./" === url)
    )
      return "" + hash;
    for (; url !== newURL; )
      (newURL = url), (url = url.replace(/[\/][^\/]+\/+\.\.\//, "/"));
    return (url = url.replace(/^.[^\/]+\/+\.\.\//, "")), url + hash;
  }),
  (String._div = document.createElement("div"));
var Page = {
  init: function () {
    (Page.devlib_url = document
      .getElementById("ssi_LibraryTitle")
      .href.replace(/navigation\//g, "")),
      (Page.url = document.location.pathname.directoryForURL()),
      (Page.root = (
        document.getElementById("book-root")
          ? Page.url + document.getElementById("book-root").content
          : ""
      ).cleanUpURL()),
      (Page.language = document.getElementsByTagName("html")
        ? document.getElementsByTagName("html")[0].getAttribute("lang")
        : "");
  },
  didLoad: function () {},
};
(ContentFrame.prototype.handleEvent = function (event) {
  switch (event.type) {
    case "load":
      this.pageDidLoad(event);
      break;
    case "unload":
      var _this = this;
      setTimeout(function () {
        Event.observe(
          _this.element,
          "load",
          _this.handleEvent.bindAsEventListener(_this),
        );
      }, 0);
      break;
    default:
      console.error("Unexpected event passed to ContentFrame: ".event.type);
  }
}),
  (ContentFrame.prototype.pageDidLoad = function () {
    (this.doc = window.document),
      this.doc.location.pathname.replace(
        window.location.pathname.directoryForURL(),
        "",
      ) + this.doc.location.hash,
      Page.init(),
      TOC.load(),
      LegacyNotice.init(),
      Prototype.Browser.Xcode || this.doc.body.focus();
  }),
  (ContentFrame.prototype.loadPage = function () {
    var currentLocation = window.unescape(
        window.location.hash.replace(/^#/, ""),
      ),
      self = this;
    currentLocation &&
      (currentLocation.match(/^[A-Za-z0-9_]+\//)
        ? self.doc
          ? self.doc.location.replace(currentLocation)
          : (self.element.src = currentLocation)
        : window.location.replace("./"));
  });
var LegacyNotice = {
    init: function () {
      if (window.document.getElementById("legacyOuterWrapper")) {
        var now,
          lastSawLegacyNotice = Cookie.get(
            "LastSawLegacyNoticeFor" +
              window.document.getElementById("identifier").content,
          );
        lastSawLegacyNotice
          ? ((lastSawLegacyNotice = new Date(lastSawLegacyNotice)),
            (now = new Date()),
            now - lastSawLegacyNotice > 864e5 && LegacyNotice.show())
          : LegacyNotice.show();
      }
    },
    show: function () {
      window.document.getElementById("legacyOuterWrapper").addClassName("show"),
        window.document
          .getElementById("LegacyNoticeCloseBox")
          .observe("click", LegacyNotice.close);
    },
    close: function () {
      window.document
        .getElementById("legacyOuterWrapper")
        .removeClassName("show"),
        window.document
          .getElementById("LegacyNoticeCloseBox")
          .stopObserving("click", LegacyNotice.close),
        Cookie.set({
          cName:
            "LastSawLegacyNoticeFor" +
            window.document.getElementById("identifier").content,
          cValue: new Date(),
          cExpiry: COOKIE_EXPIRE_DATE,
        });
    },
  },
  HistoryState = {
    init: function () {
      (HistoryState.title = window.document.title),
        document.getElementById("contents") &&
          (HistoryState.innerHTML =
            window.document.getElementById("contents").innerHTML),
        (HistoryState.href = window.location.pathname + window.location.hash),
        (HistoryState.firstLoad = !0),
        (HistoryState.backFromTask = !1);
      for (
        var meta_tag,
          new_meta_tags = document.getElementsByTagName("meta"),
          meta_tags = {},
          i = 0;
        new_meta_tags.length > i;
        i++
      )
        (meta_tag = new_meta_tags[i]),
          meta_tag.id && "book-resource-type" === meta_tag.id
            ? (meta_tags["book-resource-type"] =
                document.getElementById("book-resource-type").content)
            : meta_tag.id && "identifier" === meta_tag.id
              ? (meta_tags.identifier =
                  document.getElementById("identifier").content)
              : meta_tag.id && "document-version" === meta_tag.id
                ? (meta_tags["document-version"] =
                    document.getElementById("document-version").content)
                : meta_tag.id && "chapterId" === meta_tag.id
                  ? (meta_tags.chapterId =
                      document.getElementById("chapterId").content)
                  : meta_tag.id && "date" === meta_tag.id
                    ? (meta_tags.date = document.getElementById("date").content)
                    : meta_tag.id && "book-root" === meta_tag.id
                      ? (meta_tags["book-root"] =
                          document.getElementById("book-root").content)
                      : meta_tag.id && "book-json" === meta_tag.id
                        ? ((meta_tags["book-json"] =
                            document.getElementById("book-json").content),
                          "book.json" === meta_tags["book-json"] &&
                            (meta_tags["book-json"] = "./book.json"))
                        : meta_tag.id && "book-title" === meta_tag.id
                          ? (meta_tags["book-title"] =
                              document.getElementById("book-title").content)
                          : meta_tag.id && "book-name" === meta_tag.id
                            ? (meta_tags["book-name"] =
                                document.getElementById("book-name").content)
                            : meta_tag.id && "devcenter" === meta_tag.id
                              ? (meta_tags.devcenter =
                                  document.getElementById("devcenter").content)
                              : meta_tag.id && "devcenter-url" === meta_tag.id
                                ? (meta_tags["devcenter-url"] =
                                    document.getElementById(
                                      "devcenter-url",
                                    ).content)
                                : meta_tag.id && "reflib" === meta_tag.id
                                  ? (meta_tags.reflib =
                                      document.getElementById("reflib").content)
                                  : meta_tag.id &&
                                      "book-assignments" === meta_tag.id
                                    ? (meta_tags["book-assignments"] =
                                        document.getElementById(
                                          "book-assignments",
                                        ).content)
                                    : meta_tag.id &&
                                        "IndexTitle" === meta_tag.id
                                      ? (meta_tags.IndexTitle =
                                          document.getElementById(
                                            "IndexTitle",
                                          ).content)
                                      : meta_tag.id &&
                                        "resources-uri" === meta_tag.id &&
                                        (meta_tags["resources-uri"] =
                                          document.getElementById(
                                            "resources-uri",
                                          ).content);
      HistoryState.meta_tags = meta_tags;
      for (
        var current_link_tags = document.getElementsByTagName("link"),
          link_tags = {},
          i = 0,
          length = current_link_tags.length;
        length > i;
        i++
      ) {
        var link_tag = current_link_tags[i];
        link_tag.id && "book-index-page" === link_tag.id
          ? (link_tags["book-index-page"] =
              document.getElementById("book-index-page").href)
          : link_tag.id && "next-page" === link_tag.id
            ? (link_tags["next-page"] =
                document.getElementById("next-page").href)
            : link_tag.id &&
              "previous-page" === link_tag.id &&
              (link_tags["previous-page"] =
                document.getElementById("previous-page").href);
      }
      (HistoryState.link_tags = link_tags),
        Prototype.Browser.Xcode ||
          (document.getElementById("contents") &&
            document.getElementById("contents").focus());
      var stylesheets = document.querySelectorAll("link[rel='stylesheet']"),
        customStylesheets = [],
        re = RegExp("(screen.css|feedback.css|xcode4.css|xcode5.css)"),
        forEach = Array.prototype.forEach;
      forEach.call(stylesheets, function (css) {
        -1 === css.href.search(re) &&
          ((css.id = "custom"), customStylesheets.push(css));
      }),
        (HistoryState.css = customStylesheets);
    },
    parseLinks: function () {
      for (
        var els = window.document.getElementsByTagName("a"),
          bookname = document
            .getElementById("book-name")
            .getAttribute("content"),
          links_within_book = [],
          i = 0;
        els.length > i;
        i++
      ) {
        var link = els.item(i);
        if (
          "ssi_LibraryTitle" === link.id ||
          "ssi_PreRelease" === link.id ||
          "ssi_Legacy" === link.id
        )
          link.href = link.href;
        else if (
          "ssi_AppleDeveloperConnection" === link.id ||
          "top" === link.id ||
          "INDEX" === link.id ||
          "roadmap_child" === link.id ||
          "Sample_link" === link.id
        );
        else if ("button" === link.getAttribute("role"));
        else if ("" === link.href);
        else if (link.hostname !== location.hostname);
        else if (
          link.parentElement &&
          link.parentElement.parentElement &&
          "inlinerecipe-name task-name" ===
            link.parentElement.parentElement.getAttribute("class")
        );
        else if (
          link.parentElement &&
          link.parentElement &&
          "recipeLink" === link.parentElement.getAttribute("class")
        );
        else if (
          link.parentElement &&
          link.parentElement &&
          "pediaLink" === link.parentElement.getAttribute("class") &&
          "Pedia" !== Book.type
        );
        else if ("_blank" === link.target || "external" === link.rel);
        else if (/^[a-z]+:\/\//i.test(link.getAttribute("href")));
        else {
          var href = link.href,
            re = RegExp("(MobileHIG|UIKitUICatalog|TransitionGuide)"),
            isJazz = -1 === href.search(re) ? !1 : !0;
          isJazz === !1 &&
            (-1 !== href.search(bookname) && links_within_book.push(link),
            link.addEventListener("click", TOC.processEvent, !0));
        }
      }
    },
  },
  TOC = {
    initExpandable: function (sections) {
      var i,
        apple_ref,
        chapter,
        section = function (section) {
          return function () {
            return section;
          };
        };
      for (i = 0; sections.length > i; i++)
        (apple_ref = sections[i].down("a").getAttribute("href")),
          (chapter = apple_ref.slice(
            apple_ref.lastIndexOf("/") + 1,
            apple_ref.length,
          )),
          "open" === Cookie.get(chapter) &&
            TOC.toggleSection({ element: section(sections[i]) });
      return i;
    },
    changeCurrentLocation: function (event) {
      return (
        Page.currentLocation &&
          Page.currentLocation.removeClassName("currentLocation"),
        (Page.currentLocation = Event.element(event)),
        Page.currentLocation.addClassName("currentLocation")
      );
    },
    processFileLinks: function () {
      return (
        Book.PDF && Book.PDF.href
          ? (document
              .getElementById("PDF_link")
              .setAttribute("href", Page.root + Book.PDF.href),
            document.getElementById("PDF_link").addClassName("visible"))
          : document.getElementById("PDF_link").removeClassName("visible"),
        Book.companionFiles
          ? (document
              .getElementById("Companion_link")
              .setAttribute("href", Page.root + Book.companionFiles),
            document.getElementById("Companion_link").addClassName("visible"))
          : document
              .getElementById("Companion_link")
              .removeClassName("visible"),
        [
          document.getElementById("Companion_link"),
          document.getElementById("PDF_link"),
        ]
      );
    },
    setupSampleCodeButton: function () {
      Book.sampleCode
        ? (document
            .getElementById("Sample_link")
            .setAttribute("href", Page.root + Book.sampleCode),
          Prototype.Browser.Xcode &&
            (document.getElementById("Sample_button").innerText =
              "Open Project in Xcode"),
          document.getElementById("downloadSample_button").show())
        : document.getElementById("downloadSample_button").hide();
    },
    isXcodeRecipe: function () {
      return (
        Prototype.Browser.Xcode &&
        $("book-resource-type") &&
        "Recipe" === $("book-resource-type").content
      );
    },
    addSpecialTargetsToLinksInXcode4: function () {
      if (
        5 > Prototype.Browser.XcodeVersion &&
        Prototype.Browser.Xcode4OrNewer
      ) {
        var links = document.getElementsByTagName("a");
        Array.prototype.forEach.call(links, function (link) {
          link.hasAttribute("target") &&
            "_blank" === link.target &&
            (link.target = link.target.replace(
              /_blank/,
              "__xcodeOpenInBrowser",
            ));
        });
      }
    },
    build: function (sections) {
      var i,
        hasChildren,
        isOpen,
        section,
        tocHTML = "";
      for (i = 0; sections.length > i; i++)
        (section = sections[i]),
          section.href &&
            "<" != section.href.charAt(0) &&
            ((hasChildren =
              section.sections && section.sections.length > 0
                ? "children"
                : ""),
            (isOpen = Cookie.get(section.aref) ? Cookie.get(section.aref) : ""),
            (tocHTML +=
              '<li class="' +
              hasChildren +
              " " +
              isOpen +
              (section.aref ? '" data-aref="' + section.aref + '"' : '"') +
              (hasChildren ? 'role="treeitem"' : "") +
              ">" +
              (hasChildren
                ? '<span class="disclosure"></span>'
                : '<span class="nodisclosure"></span>') +
              '<span class="sectionName">' +
              (section.href
                ? '<a href="' +
                  (
                    (section.href.isHTTPurl() ? "" : Page.root) + section.href
                  ).cleanUpURL() +
                  '">'
                : "") +
              section.title.escapeHTML() +
              (section.href ? "</a></span>" : "</span>") +
              ("" !== hasChildren
                ? "<ul>" + TOC.build(sections[i].sections) + "</ul>"
                : "") +
              "</li>"));
      return tocHTML;
    },
    updateProgress: function (event) {
      event.lengthComputable && event.loaded / event.total;
    },
    transferComplete: function () {
      Page.didLoad(!0);
    },
    transferFailed: function () {
      Page.didLoad(!1);
    },
    transferCanceled: function () {
      Page.didLoad(!1);
    },
    processEvent: function (e) {
      if (!e.metaKey && !e.ctrlKey) {
        e.preventDefault();
        var bookname = document
          .getElementById("book-name")
          .getAttribute("content");
        if (
          (e.target.rel && (HistoryState.backFromTask = !0),
          e.target.pathname !== window.location.pathname)
        ) {
          RegExp(bookname).test(e.target.pathname);
          var httpRequest;
          (httpRequest = new XMLHttpRequest()),
            httpRequest.addEventListener("progress", TOC.updateProgress, !1),
            httpRequest.addEventListener("load", TOC.transferComplete, !1),
            httpRequest.addEventListener("error", TOC.transferFailed, !1),
            httpRequest.addEventListener("abort", TOC.transferCanceled, !1),
            (httpRequest.onreadystatechange = function () {
              if (4 === httpRequest.readyState) {
                var state_obj = {},
                  fullPathWithHash = e.target.pathname + e.target.hash;
                history.pushState(
                  null,
                  window.document.title,
                  fullPathWithHash,
                );
                var HTML = httpRequest.responseText,
                  tempDiv = document.createElement("div");
                tempDiv.innerHTML = HTML;
                var title = tempDiv.getElementsByTagName("title")[0].innerHTML,
                  article = tempDiv.getElementsByTagName("article")[0],
                  newFirstLinkJumpToName = tempDiv.getElementsByTagName("a")[0],
                  firstLinkJumpToName = document.getElementsByTagName("a")[0];
                newFirstLinkJumpToName &&
                  ((firstLinkJumpToName.name = newFirstLinkJumpToName.name),
                  (firstLinkJumpToName.title = newFirstLinkJumpToName.title));
                for (
                  var meta_tag,
                    new_meta_tags = tempDiv.getElementsByTagName("meta"),
                    meta_tags = {},
                    i = 0;
                  new_meta_tags.length > i;
                  i++
                )
                  (meta_tag = new_meta_tags[i]),
                    meta_tag.id && "book-resource-type" === meta_tag.id
                      ? ((document.getElementById(
                          "book-resource-type",
                        ).content = meta_tag.content),
                        (meta_tags["book-resource-type"] =
                          document.getElementById(
                            "book-resource-type",
                          ).content))
                      : meta_tag.id && "identifier" === meta_tag.id
                        ? ((document.getElementById("identifier").content =
                            meta_tag.content),
                          (meta_tags.identifier =
                            document.getElementById("identifier").content))
                        : meta_tag.id && "document-version" === meta_tag.id
                          ? ((document.getElementById(
                              "document-version",
                            ).content = meta_tag.content),
                            (meta_tags["document-version"] =
                              document.getElementById(
                                "document-version",
                              ).content))
                          : meta_tag.id && "chapterId" === meta_tag.id
                            ? ((document.getElementById("chapterId").content =
                                meta_tag.content),
                              (meta_tags.chapterId =
                                document.getElementById("chapterId").content))
                            : meta_tag.id && "date" === meta_tag.id
                              ? ((document.getElementById("date").content =
                                  meta_tag.content),
                                (meta_tags.date =
                                  document.getElementById("date").content))
                              : meta_tag.id && "book-root" === meta_tag.id
                                ? ((document.getElementById(
                                    "book-root",
                                  ).content = meta_tag.content),
                                  (meta_tags["book-root"] =
                                    document.getElementById(
                                      "book-root",
                                    ).content))
                                : meta_tag.id && "book-json" === meta_tag.id
                                  ? ((document.getElementById(
                                      "book-json",
                                    ).content = meta_tag.content),
                                    (meta_tags["book-json"] =
                                      document.getElementById(
                                        "book-json",
                                      ).content))
                                  : meta_tag.id && "book-title" === meta_tag.id
                                    ? ((document.getElementById(
                                        "book-title",
                                      ).content = meta_tag.content),
                                      (meta_tags["book-title"] =
                                        document.getElementById(
                                          "book-title",
                                        ).content))
                                    : meta_tag.id && "book-name" === meta_tag.id
                                      ? ((document.getElementById(
                                          "book-name",
                                        ).content = meta_tag.content),
                                        (meta_tags["book-name"] =
                                          document.getElementById(
                                            "book-name",
                                          ).content))
                                      : meta_tag.id &&
                                          "devcenter" === meta_tag.id
                                        ? ((document.getElementById(
                                            "devcenter",
                                          ).content = meta_tag.content),
                                          (meta_tags.devcenter =
                                            document.getElementById(
                                              "devcenter",
                                            ).content))
                                        : meta_tag.id &&
                                            "devcenter-url" === meta_tag.id
                                          ? ((document.getElementById(
                                              "devcenter-url",
                                            ).content = meta_tag.content),
                                            (meta_tags["devcenter-url"] =
                                              document.getElementById(
                                                "devcenter-url",
                                              ).content))
                                          : meta_tag.id &&
                                              "reflib" === meta_tag.id
                                            ? ((document.getElementById(
                                                "reflib",
                                              ).content = meta_tag.content),
                                              (meta_tags.reflib =
                                                document.getElementById(
                                                  "reflib",
                                                ).content))
                                            : meta_tag.id &&
                                                "book-assignments" ===
                                                  meta_tag.id
                                              ? ((document.getElementById(
                                                  "book-assignments",
                                                ).content = meta_tag.content),
                                                (meta_tags["book-assignments"] =
                                                  document.getElementById(
                                                    "book-assignments",
                                                  ).content))
                                              : meta_tag.id &&
                                                  "IndexTitle" === meta_tag.id
                                                ? ((document.getElementById(
                                                    "IndexTitle",
                                                  ).content = meta_tag.content),
                                                  (meta_tags.IndexTitle =
                                                    document.getElementById(
                                                      "IndexTitle",
                                                    ).content))
                                                : meta_tag.id &&
                                                  "resources-uri" ===
                                                    meta_tag.id &&
                                                  ((document.getElementById(
                                                    "resources-uri",
                                                  ).content = meta_tag.content),
                                                  (meta_tags["resources-uri"] =
                                                    document.getElementById(
                                                      "resources-uri",
                                                    ).content));
                for (
                  var current_link_tags = tempDiv.getElementsByTagName("link"),
                    link_tags = {},
                    i = 0,
                    length = current_link_tags.length;
                  length > i;
                  i++
                ) {
                  var link_tag = current_link_tags[i];
                  link_tag.id && "book-index-page" === link_tag.id
                    ? ((document.getElementById("book-index-page").href =
                        link_tag.href),
                      (link_tags["book-index-page"] =
                        document.getElementById("book-index-page").href))
                    : link_tag.id && "next-page" === link_tag.id
                      ? ((document.getElementById("next-page").href =
                          link_tag.href),
                        (link_tags["next-page"] =
                          document.getElementById("next-page").href))
                      : link_tag.id &&
                        "previous-page" === link_tag.id &&
                        ((document.getElementById("previous-page").href =
                          link_tag.href),
                        (link_tags["previous-page"] =
                          document.getElementById("previous-page").href));
                }
                var stylesheets = tempDiv.querySelectorAll(
                    "link[rel='stylesheet']",
                  ),
                  customStylesheets = [],
                  re = RegExp(
                    "(screen.css|feedback.css|xcode4.css|xcode5.css)",
                  ),
                  forEach = Array.prototype.forEach;
                forEach.call(stylesheets, function (css) {
                  if (-1 === css.href.search(re)) {
                    var customCSS = {};
                    (customCSS.id = "custom"),
                      (css.id = "custom"),
                      (customCSS.href = css.href),
                      (customCSS.type = css.type),
                      (customCSS.rel = css.rel),
                      customStylesheets.push(customCSS);
                  }
                });
                var currentCSS = document.querySelectorAll(
                  "link[rel='stylesheet']",
                );
                if (1 === customStylesheets.length) {
                  var head = document.getElementsByTagName("head")[0],
                    exists = !1;
                  if (
                    (forEach.call(currentCSS, function (css) {
                      css.href === customStylesheets[0].href && (exists = !0);
                    }),
                    exists === !1)
                  ) {
                    var csslink = document.createElement("link");
                    csslink.setAttribute("rel", "stylesheet"),
                      csslink.setAttribute("type", "text/css"),
                      csslink.setAttribute("href", customStylesheets[0].href),
                      csslink.setAttribute("id", "custom"),
                      head.appendChild(csslink);
                  }
                }
                (state_obj.page = article.innerHTML),
                  (state_obj.title = title),
                  (state_obj.meta_tags = meta_tags),
                  (state_obj.link_tags = link_tags),
                  (state_obj.css_tags = customStylesheets),
                  history.replaceState(
                    state_obj,
                    document.title,
                    fullPathWithHash,
                  ),
                  (HistoryState.firstLoad = !1),
                  (document.title = title);
                var title_tag = document.getElementById("title"),
                  h1_tag = title_tag.getElementsByTagName("h1")[0];
                (h1_tag.innerHTML = meta_tags["book-title"]),
                  Page.init(),
                  TOC.load(),
                  TOC.processFileLinks(),
                  TOC.setupSampleCodeButton(),
                  setupJumpTo();
                var old_article = window.document.getElementById("contents");
                if (
                  (old_article.innerHTML,
                  (old_article.innerHTML = article.innerHTML),
                  HistoryState.parseLinks(),
                  new Pedia(),
                  new Recipe(),
                  Tasks.init(),
                  DevPubs.constructNativeVideos(),
                  "undefined" != typeof PageTracker)
                ) {
                  var eventDescription = {};
                  document.referrer &&
                    (eventDescription.referrer = document.referrer),
                    (eventDescription.identifier =
                      document.getElementById("identifier").content),
                    PageTracker.logEvent("ajaxPageLoad", eventDescription);
                }
                Prototype.Browser.Xcode ||
                  (document.getElementById("contents") &&
                    document.getElementById("contents").focus());
                var hashName = e.target.hash.substring(1, e.target.hash.length),
                  jumpName = document.getElementById("pageTitle"),
                  iOS = navigator.userAgent.match(/(iPad|iPhone|iPod)/g)
                    ? !0
                    : !1;
                jumpName && TOC.isXcodeRecipe()
                  ? jumpName.scrollIntoView(!1)
                  : 0 === hashName.length
                    ? jumpName.scrollIntoView(!1)
                    : 1 !== hashName.indexOf("//apple_ref/")
                      ? ((jumpName = document.getElementsByName(hashName)),
                        jumpName.length > 0 && jumpName[0].scrollIntoView(!iOS))
                      : ((jumpName = document.getElementById(hashName)),
                        null !== jumpName &&
                          void 0 !== jumpName &&
                          jumpName.scrollIntoView(!iOS));
              }
              TOC.addSpecialTargetsToLinksInXcode4();
            }),
            httpRequest.open("GET", e.target.href),
            httpRequest.send();
        } else {
          if (e.target.hash === window.location.hash) {
            Prototype.Browser.Xcode ||
              document.getElementById("contents").focus();
            var targetHashName = e.target.hash.substring(
                1,
                e.target.hash.length,
              ),
              jumpName = document.getElementsByName(targetHashName);
            return (
              jumpName.length > 0 &&
                jumpName[0].scrollIntoView(!TOC.isXcodeRecipe()),
              void 0
            );
          }
          var targetHashName = e.target.hash.substring(1, e.target.hash.length),
            jumpName = document.getElementsByName(targetHashName);
          if (jumpName.length > 0) {
            jumpName[0].scrollIntoView(!TOC.isXcodeRecipe());
            var state_obj = {},
              meta_tags = {};
            (meta_tags["book-resource-type"] =
              document.getElementById("book-resource-type").content),
              (meta_tags.identifier =
                document.getElementById("identifier").content),
              (meta_tags["document-version"] =
                document.getElementById("document-version").content),
              (meta_tags.chapterId =
                document.getElementById("chapterId").content),
              (meta_tags.date = document.getElementById("date").content),
              (meta_tags["book-root"] =
                document.getElementById("book-root").content),
              (meta_tags["book-json"] =
                document.getElementById("book-json").content),
              (meta_tags["book-title"] =
                document.getElementById("book-title").content),
              (meta_tags["book-name"] =
                document.getElementById("book-name").content),
              (meta_tags.devcenter =
                document.getElementById("devcenter").content),
              (meta_tags["devcenter-url"] =
                document.getElementById("devcenter-url").content),
              (meta_tags.reflib = document.getElementById("reflib").content),
              (meta_tags["book-assignments"] =
                document.getElementById("book-assignments").content),
              (meta_tags.IndexTitle =
                document.getElementById("IndexTitle").content),
              (meta_tags["resources-uri"] =
                document.getElementById("resources-uri").content);
            var article = document.getElementsByTagName("article")[0];
            (state_obj.page = article.innerHTML),
              (state_obj.meta_tags = meta_tags),
              (state_obj.scrollTop = article.scrollTop),
              (state_obj.title = window.document.title);
            var fullPathWithHash = e.target.pathname + e.target.hash;
            history.pushState(
              state_obj,
              window.document.title,
              fullPathWithHash,
            ),
              (HistoryState.firstLoad = !1);
          }
        }
      }
    },
    didLoad: function (transport) {
      if (
        ((Book = transport.responseText.evalJSON()),
        !Book.sections ||
          !Book.shouldDisplayTOC ||
          (Prototype.Browser.Xcode && Prototype.Browser.Xcode4OrNewer))
      )
        TOC.hide(!0),
          document.getElementById("toc_button").hide(),
          TOC.processFileLinks(),
          HistoryState.parseLinks(),
          TOC.addSpecialTargetsToLinksInXcode4();
      else {
        "false" === Cookie.get("toc-visible") ? TOC.hide() : TOC.show(),
          document.getElementById("toc_button").observe("click", TOC.toggle),
          document.getElementById("toc_button").show();
        var tocHTML = TOC.build(Book.sections);
        Book.relatedBooks.length > 0 &&
          Book.relatedBooks.each(function (b, idx) {
            var relatedSections = TOC.build(b.sections);
            "" !== relatedSections &&
              ((tocHTML +=
                '<ul id="TOC_relatedBooks_' + idx + '" class="related">'),
              (tocHTML += "<h3>" + b.title + "</h3>"),
              (tocHTML += relatedSections),
              (tocHTML += "</ul>"));
          }),
          (document.getElementById("toc").innerHTML = tocHTML),
          document.getElementById("toc").observe("click", TOC.toggleSection),
          HistoryState.parseLinks();
      }
      (window.onpopstate = function (e) {
        if (e.state) {
          var data = e.state,
            meta_tags = data.meta_tags,
            link_tags = data.link_tags,
            css_tags = data.css_tags,
            old_article = window.document.getElementById("contents");
          if (
            (old_article.innerHTML,
            (old_article.innerHTML = data.page),
            e.state.scrollTop,
            HistoryState.backFromTask)
          ) {
            var h1PageTitle = document.getElementById("pageTitle");
            h1PageTitle.scrollIntoView(!0), (HistoryState.backFromTask = !1);
          }
          TOC.processFileLinks(), TOC.setupSampleCodeButton(), setupJumpTo();
          for (
            var meta_tag_ids = Object.keys(meta_tags),
              i = 0,
              length = meta_tag_ids.length;
            length > i;
            i++
          ) {
            var meta_tag_content = meta_tags[meta_tag_ids[i]];
            document.getElementById(meta_tag_ids[i]).content = meta_tag_content;
          }
          Page.init(), TOC.load();
          var title_tag = document.getElementById("title"),
            h1_tag = title_tag.getElementsByTagName("h1")[0];
          (h1_tag.innerHTML = document.getElementById("book-title").content),
            (document.title = h1_tag.innerHTML);
          for (
            var link_tag_ids = Object.keys(link_tags),
              i = 0,
              length = link_tag_ids.length;
            length > i;
            i++
          ) {
            var link_tag_href = link_tags[link_tag_ids[i]];
            document.getElementById(link_tag_ids[i]).href = link_tag_href;
          }
          var css = void 0;
          void 0 != css_tags && 1 === css_tags.length && (css = css_tags[0]);
          var head = document.getElementsByTagName("head")[0];
          if (void 0 === css)
            (css = document.querySelector("#custom")),
              null !== css && head.removeChild(css);
          else if (
            ((custom_css = document.querySelector("#custom")),
            null !== custom_css)
          )
            custom_css.href = css.href;
          else {
            var csslink = document.createElement("link");
            csslink.setAttribute("rel", "stylesheet"),
              csslink.setAttribute("type", "text/css"),
              csslink.setAttribute("href", css.href),
              csslink.setAttribute("id", "custom"),
              head.appendChild(csslink);
          }
          var hashName = window.location.hash.substring(
              1,
              window.location.hash.length,
            ),
            jumpName = document.getElementById("pageTitle");
          0 === hashName.length
            ? jumpName.scrollIntoView(!1)
            : 1 !== hashName.indexOf("//apple_ref/")
              ? ((jumpName = document.getElementsByName(hashName)),
                jumpName.length > 0 && jumpName[0].scrollIntoView())
              : ((jumpName = document.getElementById(hashName)),
                null !== jumpName &&
                  void 0 !== jumpName &&
                  jumpName.scrollIntoView());
        } else if (
          HistoryState.href !==
          window.location.pathname + window.location.hash
        );
        else {
          var old_article = window.document.getElementById("contents"),
            meta_tags = HistoryState.meta_tags,
            link_tags = HistoryState.link_tags,
            css_tags = HistoryState.css;
          old_article.innerHTML = HistoryState.innerHTML;
          for (
            var meta_tag_ids = Object.keys(meta_tags),
              i = 0,
              length = meta_tag_ids.length;
            length > i;
            i++
          ) {
            var meta_tag_content = meta_tags[meta_tag_ids[i]];
            document.getElementById(meta_tag_ids[i]).content = meta_tag_content;
          }
          var title_tag = document.getElementById("title"),
            h1_tag = title_tag.getElementsByTagName("h1")[0];
          (h1_tag.innerHTML = document.getElementById("book-title").content),
            (document.title = h1_tag.innerHTML);
          for (
            var link_tag_ids = Object.keys(link_tags),
              i = 0,
              length = link_tag_ids.length;
            length > i;
            i++
          ) {
            var link_tag_href = link_tags[link_tag_ids[i]];
            document.getElementById(link_tag_ids[i]).href = link_tag_href;
          }
          var css = void 0;
          void 0 != css_tags && 1 === css_tags.length && (css = css_tags[0]);
          var head = document.getElementsByTagName("head")[0];
          if (void 0 === css)
            (css = document.querySelector("#custom")),
              null !== css && head.removeChild(css);
          else if (
            ((custom_css = document.querySelector("#custom")),
            null !== custom_css)
          )
            custom_css.href = css.href;
          else {
            var csslink = document.createElement("link");
            csslink.setAttribute("rel", "stylesheet"),
              csslink.setAttribute("type", "text/css"),
              csslink.setAttribute("href", css.href),
              csslink.setAttribute("id", "custom"),
              head.appendChild(csslink);
          }
          if (HistoryState.backFromTask) {
            var h1PageTitle = document.getElementById("pageTitle");
            h1PageTitle.scrollIntoView(!0), (HistoryState.backFromTask = !1);
          }
          (window.document.title = HistoryState.title),
            Page.init(),
            TOC.load(),
            TOC.processFileLinks(),
            TOC.setupSampleCodeButton(),
            HistoryState.parseLinks();
          var hashName = window.location.hash.substring(
              1,
              window.location.hash.length,
            ),
            jumpName = document.getElementById("pageTitle");
          0 === hashName.length
            ? jumpName.scrollIntoView()
            : 1 !== hashName.indexOf("//apple_ref/")
              ? ((jumpName = document.getElementsByName(hashName)),
                jumpName.length > 0 && jumpName[0].scrollIntoView())
              : ((jumpName = document.getElementById(hashName)),
                null !== jumpName &&
                  void 0 !== jumpName &&
                  jumpName.scrollIntoView());
        }
        setupJumpTo(), Tasks.init(), new Pedia(), new Recipe();
      }),
        Page.init(),
        TOC.processFileLinks(),
        TOC.setupSampleCodeButton(),
        setupJumpTo(),
        TOC.addSpecialTargetsToLinksInXcode4();
    },
    load: function () {
      if (
        window.document.getElementById("book-json") &&
        "" !== window.document.getElementById("book-json").content
      ) {
        var book_json_path =
          window.document.getElementById("book-json").content;
        return new Ajax.Request(book_json_path, {
          method: "get",
          onSuccess: TOC.didLoad,
          onFailure: function () {
            TOC.hide(!0),
              document.getElementById("toc_button").hide(),
              document.getElementById("PDF_link").removeClassName("visible"),
              document
                .getElementById("Companion_link")
                .removeClassName("visible"),
              document.getElementById("downloadSample_button").hide();
          },
        });
      }
      (Book = {}),
        TOC.hide(!0),
        document.getElementById("toc_button").hide(),
        TOC.addSpecialTargetsToLinksInXcode4();
    },
    hide: function (doNotSetCookie) {
      return (
        document.getElementById("contents").removeClassName("isShowingTOC"),
        document.getElementById("tocContainer").removeClassName("isShowingTOC"),
        document
          .getElementById("toc_button")
          .down("button")
          .removeClassName("open"),
        doNotSetCookie
          ? !1
          : Cookie.set({
              cName: "toc-visible",
              cValue: "false",
              cExpiry: COOKIE_EXPIRE_DATE,
              cPath: "/",
            })
      );
    },
    show: function () {
      document.getElementById("contents").addClassName("isShowingTOC"),
        document.getElementById("tocContainer").addClassName("isShowingTOC"),
        document
          .getElementById("toc_button")
          .down("button")
          .addClassName("open"),
        Cookie.expire("toc-visible");
    },
    toggle: function () {
      var tocElement = document.getElementById("tocContainer");
      return (
        tocElement.hasClassName("isShowingTOC") ? TOC.hide() : TOC.show(),
        tocElement
      );
    },
    toggleSection: function (event) {
      if (
        event.element().hasClassName("children") ||
        event.element().hasClassName("disclosure")
      ) {
        var element = event.element().hasClassName("children")
          ? event.element()
          : event.element().parentNode;
        return element.hasClassName("open")
          ? (element.removeClassName("open"),
            Cookie.expire(element.getAttribute("data-aref")))
          : (element.addClassName("open"),
            Cookie.set({
              cName: element.getAttribute("data-aref"),
              cValue: "open",
              cExpiry: COOKIE_EXPIRE_DATE,
              cPath: "/",
            }));
      }
      return !0;
    },
  },
  PageController = {
    next: function () {
      return window.document.getElementById("next-page")
        ? (window.document.location =
            window.document.getElementById("next-page").href)
        : null;
    },
    previous: function () {
      return window.document.getElementById("previous-page")
        ? (window.document.location =
            window.document.getElementById("previous-page").href)
        : null;
    },
  },
  QuickLook = {
    create: function () {
      (QuickLook.greyout = document.createElement("div")),
        (QuickLook.greyout.id = "QuickLookGreyout"),
        (QuickLook.window = document.createElement("div")),
        (QuickLook.window.id = "QuickLookWindow"),
        (QuickLook.window.innerHTML =
          '<div id="QuickLookHeader"></div><div id="QuickLookBody"></div><div id="QuickLookFooter"></div>'),
        (QuickLook.closeBox = document.createElement("div")),
        (QuickLook.closeBox.id = "QuickLookCloseBox"),
        document.body.appendChild(QuickLook.greyout),
        document.body.appendChild(QuickLook.window),
        document.body.appendChild(QuickLook.closeBox);
    },
    updateQuickLookLinks: function (recipeContent, root_url) {
      var links = recipeContent.getElementsByTagName("a"),
        forEach = Array.prototype.forEach;
      forEach.call(links, function (link) {
        var href = link.getAttribute("href");
        null !== href &&
          ((href = root_url + href),
          link.setAttribute("href", href),
          link.setAttribute("target", "_blank"));
      });
    },
    updateQuickLookContent: function (recipeContent, root_url, isPedia) {
      var forEach = Array.prototype.forEach;
      forEach.call(
        recipeContent.querySelectorAll("img, source"),
        function (resource) {
          "taskbullet" !== resource.className &&
            (resource.src = root_url + resource.getAttribute("src"));
        },
      ),
        isPedia === !0 && new Pedia(recipeContent, { content: "DevPedia" }),
        DevPubs.constructNativeVideos(),
        forEach.call(
          recipeContent.querySelectorAll(".recipeLink, .pediaLink, .closed"),
          function (el) {
            el.classList.remove("recipeLink"),
              el.classList.remove("pediaLink"),
              el.classList.remove("closed");
          },
        );
    },
    load: function (url) {
      QuickLook.greyout || QuickLook.create();
      var recipe_root_url = url.directoryForURL();
      QuickLook.closeBox.observe("click", QuickLook.hide),
        (document.getElementById("QuickLookBody").innerHTML = ""),
        (QuickLook.article = document.createElement("section")),
        (QuickLook.article.id = "QuickLookArticle");
      var quicklookRequest = new XMLHttpRequest();
      quicklookRequest.open("GET", url, !1), quicklookRequest.send();
      var article = void 0;
      if (
        (document.getElementById("QuickLookBody"),
        200 === quicklookRequest.status)
      ) {
        var html = quicklookRequest.responseText,
          tempDiv = document.createElement("div");
        tempDiv.innerHTML = html;
        var modalIdentifier = tempDiv.querySelector("#identifier").content;
        article = tempDiv.getElementsByTagName("article")[0];
        var firstElementChild = article.firstElementChild,
          isPedia = !1;
        "recipe" === firstElementChild.id ||
          ("pediaContents" === firstElementChild.id && (isPedia = !0)),
          QuickLook.updateQuickLookContent(article, recipe_root_url, isPedia),
          QuickLook.updateQuickLookLinks(article, recipe_root_url, isPedia),
          (QuickLook.article.innerHTML = article.innerHTML),
          QuickLook.article.removeChild(
            QuickLook.article.querySelector(".copyright"),
          );
        var modal = tempDiv.querySelector("#modal"),
          feedbackButton = tempDiv.querySelector("#leave_feedback");
        if (modal && feedbackButton) {
          ($_.namespace = "#fake_modal"),
            modal.removeChild(modal.querySelector("#closebox")),
            modal.setAttribute("id", "fake_modal"),
            modal.setAttribute("class", "squished");
          var id,
            newId,
            children = modal.getElementsByTagName("*");
          Array.prototype.forEach.call(children, function (child) {
            child.hasAttribute("id") &&
              ((id = child.getAttribute("id")),
              (newId = "modal_" + id),
              child.setAttribute("id", newId));
          }),
            feedbackButton.setAttribute("id", "fake_leave_feedback");
          var copyright = tempDiv.querySelector(".copyright"),
            hr = document.createElement("hr");
          hr.setAttribute("class", "hidden"),
            QuickLook.article.appendChild(hr),
            QuickLook.article.appendChild(modal),
            QuickLook.article.appendChild(feedbackButton),
            QuickLook.article.appendChild(copyright);
        }
        if (
          (document
            .getElementById("QuickLookBody")
            .appendChild(QuickLook.article),
          DevPubs.constructNativeVideos(),
          document.getElementById("fake_leave_feedback"))
        ) {
          var feedback = new DPFeedbackWindow(),
            clickOrTap = document.ontouchend ? "touchend" : "click";
          document
            .getElementById("fake_leave_feedback")
            .addEventListener("click", function () {
              var modalMetadata = DPFeedbackMetadata.prototype;
              (modalMetadata.metaTag = function (id) {
                var el = tempDiv.querySelector("#" + id);
                return el.getAttribute("content");
              }),
                (modalMetadata.path = function () {
                  return url;
                }),
                feedback.setMetadata(modalMetadata);
              var fakeModal = document.getElementById("fake_modal"),
                fakeButton = document.getElementById("fake_leave_feedback"),
                fakeHr = document.querySelector("hr.hidden");
              if (fakeModal.classList.contains("squished")) {
                fakeModal.classList.remove("squished"),
                  fakeHr.classList.remove("hidden"),
                  fakeHr.classList.add("showing"),
                  fakeButton.classList.add("removed");
                var reps = 0,
                  interval = setInterval(function () {
                    100 > reps
                      ? (fakeModal.parentNode.scrollByLines(1), reps++)
                      : clearInterval(interval);
                  }, 3);
              } else fakeModal.classList.add("squished");
            });
          var inputs = document.querySelectorAll(
            "#modal_feedback input, #modal_feedback textarea",
          );
          Array.prototype.forEach.call(inputs, function (input) {
            input.addEventListener(
              "change",
              function () {
                feedback.valueDidChange(this);
              },
              !1,
            ),
              input.addEventListener(
                "focus",
                function () {
                  feedback.rememberFocus(this);
                },
                !1,
              );
          }),
            document.querySelector("#modal_feedback").addEventListener(
              "submit",
              function (event) {
                event.preventDefault(), feedback.send();
              },
              !1,
            ),
            document.querySelector("#modal_submit").addEventListener(
              clickOrTap,
              function () {
                feedback.send();
              },
              !1,
            );
        }
        if (
          (QuickLook.greyout.addClassName("visible"),
          QuickLook.window.addClassName("visible"),
          QuickLook.closeBox.addClassName("visible"),
          "undefined" != typeof PageTracker)
        ) {
          var eventDescription = {};
          (eventDescription.referrer = window.location.href),
            (eventDescription.identifier = modalIdentifier),
            PageTracker.logEvent("ajaxPageLoad", eventDescription);
        }
      }
      return QuickLook.window;
    },
    hide: function () {
      QuickLook.closeBox.stopObserving("click", QuickLook.hide),
        QuickLook.window.removeClassName("visible"),
        QuickLook.closeBox.removeClassName("visible"),
        QuickLook.greyout.removeClassName("visible"),
        delete $_.namespace;
      var modalFeedback = document.querySelector("#fake_modal");
      return (
        modalFeedback.parentNode.removeChild(modalFeedback), QuickLook.window
      );
    },
  };
window.addEventListener("load", function () {
  (contentFrame = new ContentFrame()),
    HistoryState.init(),
    new ADCSearchController();
});
var more = { en: "More&#8230;" };
(Pedia.prototype = {
  visible: !1,
  createPediaLinksBox: function (pediaContent) {
    var pageTitle = pediaContent.getElementsByTagName("h1");
    if (pageTitle && pageTitle[0]) {
      var i,
        pediaLinksBox = document.createElement("div"),
        pediaLinks = [],
        pediaPrerequisites =
          pediaContent.getElementsByClassName("pediaPrerequisites");
      pediaPrerequisites.length > 0 &&
        (pediaLinks = pediaLinks.concat(pediaPrerequisites[0]));
      var pediaRelated = pediaContent.getElementsByClassName("pediaRelated");
      for (
        pediaRelated.length > 0 &&
          (pediaLinks = pediaLinks.concat(pediaRelated[0])),
          pediaLinksBox.setAttribute("id", "pediaLinks"),
          i = 0;
        pediaLinks.length > i;
        i++
      )
        (pediaLinksBox.innerHTML += pediaLinks[i].innerHTML),
          pediaLinks[i].parentNode.removeChild(pediaLinks[i]);
      var section = pediaContent.querySelector("#seeAlsoSection");
      return section.insertBefore(pediaLinksBox, section.firstChild);
    }
    return !1;
  },
  addMoreLink: function (insertLocation) {
    if (this.toShow.select("a").length > 0) {
      var moreLink,
        pediaHref = this.toShow.down("a").getAttribute("data-href");
      null !== pediaHref &&
        pediaHref.split("#") &&
        (pediaHref =
          window.location.pathname.directoryForURL() + pediaHref.split("#")[0]);
      var isXcode = navigator.userAgent.indexOf("Xcode/") > -1;
      isXcode
        ? ((moreLink = document.createElement("a")),
          moreLink.setAttribute("href", pediaHref))
        : (moreLink = document.createElement("span")),
        moreLink.setAttribute("id", "peidaMore"),
        (moreLink.innerHTML = more.en),
        insertLocation.insert(moreLink),
        isXcode ||
          $("peidaMore").observe(
            "click",
            function () {
              console.log(pediaHref),
                top.frames.QuickLook.load(pediaHref),
                this.hide({ type: "force" });
            }.bindAsEventListener(this),
          );
    }
    return insertLocation;
  },
  position: function () {
    var offsetTop,
      offset =
        document.viewport.getWidth() -
        (this.visible.cumulativeOffset().left + this.popover.getWidth() + 10);
    return (
      (offset = 0 > offset ? offset - 20 : -20),
      document.viewport.getHeight() -
        this.visible.viewportOffset().top -
        this.popover.getHeight() -
        50 >
      0
        ? (this.toShow.addClassName("activePedia"),
          this.popover.clonePosition(this.visible, {
            setWidth: !1,
            setHeight: !1,
            offsetLeft: offset,
            offsetTop: this.visible.getHeight() + 2,
          }))
        : (this.toShow.addClassName("activePedia-bottom"),
          (offsetTop = -1 * this.popover.getHeight()),
          this.popover.clonePosition(this.visible, {
            setWidth: !1,
            setHeight: !1,
            offsetLeft: offset,
            offsetTop: offsetTop + 1,
          }))
    );
  },
  show: function () {
    return (
      (this.visible = this.toShow),
      ($("pediaHeader").innerHTML = this.visible.getAttribute("data-header")),
      ($("pediaBody").innerHTML = this.visible.getAttribute("data-contents")),
      this.addMoreLink($("pediaBody")),
      this.position(-4),
      this.popover.addClassName("visible"),
      this.visible
    );
  },
  click: function (event) {
    return (
      (this.toShow = event.element().hasClassName("pediaLink")
        ? event.element()
        : event.element().up(".pediaLink")),
      this.toShow
        ? (this.toShow !== this.visible && this.hide({ type: "force" }),
          this.show(),
          void 0)
        : this.hide({ type: "force" })
    );
  },
  hide: function (event) {
    this.visible &&
      ("resize" === event.type ||
        "scroll" === event.type ||
        "force" === event.type ||
        (event.element && !event.element().descendantOf(this.popover))) &&
      (this.popover.removeClassName("visible"),
      this.visible.hasClassName("activePedia")
        ? this.visible.removeClassName("activePedia")
        : this.visible.removeClassName("activePedia-bottom"),
      (this.visible = !1));
  },
}),
  (Recipe.prototype = {
    click: function (event) {
      (Prototype.Browser.Xcode &&
        $("book-resource-type") &&
        "Recipe" === $("book-resource-type").content) ||
        (event.stop(),
        (this.toShow = $(Event.element(event)).hasClassName("recipeLink")
          ? $(Event.element(event))
          : $(Event.element(event)).up(".recipeLink")),
        this.show());
    },
    show: function () {
      var recipeHref = this.toShow.down("a").href.split("#")[0];
      return top.frames.QuickLook.load(recipeHref);
    },
  }),
  window.addEventListener("load", function () {
    new Pedia(), new Recipe();
  });
var Tasks = {
  init: function () {
    if (
      ((this.tasks = document.getElementsByClassName("task")),
      this.tasks.length > 0)
    ) {
      "pediaContents" === this.tasks[0].parentNode.getAttribute("id") &&
        (this.tasks[0].classList.remove("closed"),
        (this.tasks[0].querySelector(".task-name div").innerHTML = "")),
        (this.taskNames = document.getElementsByClassName("task-name")),
        (this.inlineRecipes = document.getElementsByClassName("inlinerecipe")),
        this.loadRecipes();
      var forEach = Array.prototype.forEach,
        _this = this;
      forEach.call(this.taskNames, function (taskName) {
        taskName.addEventListener("click", function () {
          var task = this.parentNode;
          task.classList.contains("task") || (task = task.parentNode),
            "pediaContents" !== task.parentNode.getAttribute("id") &&
              _this.toggleDisclosure(task);
        });
      });
    }
    return this;
  },
  loadRecipes: function () {
    var resolve = this.resolveResourcePathsForInlineRecipes,
      forEach = Array.prototype.forEach;
    forEach.call(this.inlineRecipes, function (recipe) {
      var recipeLink = recipe.getElementsByTagName("a")[0],
        recipeContent = recipe.getElementsByClassName("task-content")[0];
      if (recipeLink) {
        var recipeURL = recipeLink.getAttribute("href");
        recipeURL || (recipeURL = recipeLink.getAttribute("name")),
          recipeLink.removeAttribute("href"),
          recipeContent.setAttribute(
            "data-source-url",
            recipeURL.directoryForURL(),
          );
        var recipe_root_url = recipeURL.directoryForURL();
        resolve(recipeContent, recipe_root_url);
      }
    });
  },
  resolveResourcePathsForInlineRecipes: function (recipeContent, root_url) {
    var forEach = Array.prototype.forEach;
    forEach.call(
      recipeContent.querySelectorAll("img, source"),
      function (resource) {
        "taskbullet" !== resource.className &&
          (resource.src = root_url + resource.getAttribute("src"));
      },
    );
    var videos = document.getElementsByTagName("video");
    forEach.call(videos, function (video) {
      video.load();
    });
  },
  toggleDisclosure: function (task) {
    task.hasClassName("closed") ? this.openTask(task) : this.closeTask(task);
  },
  openTask: function (task) {
    if (task) {
      var taskName = task.getElementsByClassName("task-name")[0],
        taskContent = task.getElementsByClassName("task-content")[0];
      taskContent.addClassName("showing"),
        (task.style.height =
          taskName.clientHeight + taskContent.clientHeight + "px"),
        task.removeClassName("closed");
    }
  },
  closeTask: function (task) {
    task &&
      !this.isOpening &&
      ((task.style.height = "35px"),
      task.addClassName("closed"),
      task.addEventListener($_animationEndEvent(), function hideContents() {
        var taskContent = task.getElementsByClassName("task-content")[0];
        taskContent.removeClassName("showing"),
          task.removeEventListener($_animationEndEvent(), hideContents);
      }));
  },
};
