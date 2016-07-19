/* -*- Mode: Java; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

// XXX Toolkit-specific preferences should be moved into toolkit.js

#filter substitution

# SYNTAX HINTS:  dashes are delimiters.  Use underscores instead.
#  The first character after a period must be alphabetic.

#ifdef XP_UNIX
#ifndef XP_MACOSX
#define UNIX_BUT_NOT_MAC
#endif
#endif

# Enable TraceMonkey
pref("javascript.options.jit.chrome", true);
pref("javascript.options.jit.content", true);

pref("general.startup.drood", true);

pref("browser.chromeURL","chrome://drood/content/");
pref("browser.hiddenWindowChromeURL", "chrome://drood/content/hiddenWindow.xul");
pref("xpinstall.dialog.confirm", "chrome://mozapps/content/xpinstall/xpinstallConfirm.xul");
pref("xpinstall.dialog.progress.skin", "chrome://mozapps/content/extensions/extensions.xul");
pref("xpinstall.dialog.progress.chrome", "chrome://mozapps/content/extensions/extensions.xul");
pref("xpinstall.dialog.progress.type.skin", "Extension:Manager");
pref("xpinstall.dialog.progress.type.chrome", "Extension:Manager");

pref("browser.download.useDownloadDir", false);
pref("browser.download.folderList", 0);
pref("browser.download.manager.showAlertOnComplete", false);
pref("browser.download.manager.closeWhenDone", true);
pref("browser.download.manager.useWindow", false);

// Developers can set this to |true| if they are constantly changing files in their 
// extensions directory so that the extension system does not constantly think that
// their extensions are being updated and thus reregistered every time the app is
// started.
pref("extensions.ignoreMTimeChanges", false);
// Enables some extra Extension System Logging (can reduce performance)
pref("extensions.logging.enabled", false);
// Hides the install button in the add-ons mgr
pref("extensions.hideInstallButton", true);
// Hides the update button in the add-ons mgr (Drood modification)
pref("extensions.hideUpdatesButton", true);

// Preferences for the Get Add-ons pane
pref("extensions.getAddons.showPane", false);
pref("extensions.getAddons.browseAddons", "https://%LOCALE%.add-ons.drood.com/%LOCALE%/%APP%");
pref("extensions.getAddons.maxResults", 5);
pref("extensions.getAddons.recommended.browseURL", "https://%LOCALE%.add-ons.drood.com/%LOCALE%/%APP%/recommended");
pref("extensions.getAddons.recommended.url", "https://add-ons.drood.com/%LOCALE%/%APP%/api/%API_VERSION%/list/featured/all/10/%OS%/%VERSION%");
pref("extensions.getAddons.search.browseURL", "https://%LOCALE%.add-ons.drood.com/%LOCALE%/%APP%/search?q=%TERMS%");
pref("extensions.getAddons.search.url", "https://add-ons.drood.com/%LOCALE%/%APP%/api/%API_VERSION%/search/%TERMS%/all/10/%OS%/%VERSION%");

// Preferences added for the Drood variant of the extension manager
pref("extensions.themes.showPane", false);
pref("extensions.plugins.showPane", false);
pref("extensions.hideRestartButton", true);

// Blocklist preferences
pref("extensions.blocklist.enabled", true);
pref("extensions.blocklist.interval", -1);
pref("extensions.blocklist.url", "https://add-ons.drood.com/blocklist/2/%APP_ID%/%APP_VERSION%/%PRODUCT%/%BUILD_ID%/%BUILD_TARGET%/%LOCALE%/%CHANNEL%/%OS_VERSION%/%DISTRIBUTION%/%DISTRIBUTION_VERSION%/");
pref("extensions.blocklist.detailsURL", "http://add-ons.drood.com/%LOCALE%/blocklist/");

// Dictionary download preference
pref("drood.dictionaries.download.url", "https://%LOCALE%.add-ons.drood.com/%LOCALE%/drood/%VERSION%/dictionaries/");

// App-specific update preferences

// Whether or not app updates are enabled
pref("app.update.enabled", false);

// This preference turns on app.update.mode and allows automatic download and
// install to take place. We use a separate boolean toggle for this to make
// the UI easier to construct.
pref("app.update.auto", true);

// Defines how the Application Update Service notifies the user about updates:
//
// AUM Set to:        Minor Releases:     Major Releases:
// 0                  download no prompt  download no prompt
// 1                  download no prompt  download no prompt if no incompatibilities
// 2                  download no prompt  prompt
//
// See chart in nsUpdateService.js.in for more details
//
pref("app.update.mode", 1);

// If set to true, the Update Service will present no UI for any event.
pref("app.update.silent", false);

// Update service URL:
pref("app.update.url", "https://angstrom.greyfirst.net/update/2/%PRODUCT%/%VERSION%/%BUILD_ID%/%BUILD_TARGET%/%LOCALE%/%CHANNEL%/%OS_VERSION%/update.xml");
// URL user can browse to manually if for some reason all update installation
// attempts fail.  TODO: Change this URL
pref("app.update.url.manual", "http://www.drood.com/");
// A default value for the "More information about this update" link
// supplied in the "An update is available" page of the update wizard. 
pref("app.update.url.details", "chrome://drood-region/locale/region.properties");

// User-settable override to app.update.url for testing purposes.
//pref("app.update.url.override", "");

// Interval: Time between checks for a new version (in seconds)
//           default=1 day
pref("app.update.interval", 86400);
// Interval: Time before prompting the user to download a new version that 
//           is available (in seconds) default=1 day
pref("app.update.nagTimer.download", 86400);
// Interval: Time before prompting the user to restart to install the latest
//           download (in seconds) default=30 minutes
pref("app.update.nagTimer.restart", 1800);
// Interval: When all registered timers should be checked (in milliseconds)
//           default=5 seconds
pref("app.update.timer", 600000);

// Whether or not we show a dialog box informing the user that the update was
// successfully applied. This is off in Firefox by default since we show a 
// upgrade start page instead! Other apps may wish to show this UI, and supply
// a whatsNewURL field in their brand.properties that contains a link to a page
// which tells users what's new in this new update.
pref("app.update.showInstalledUI", false);

// 0 = suppress prompting for incompatibilities if there are updates available
//     to newer versions of installed addons that resolve them.
// 1 = suppress prompting for incompatibilities only if there are VersionInfo
//     updates available to installed addons that resolve them, not newer
//     versions.
pref("app.update.incompatible.mode", 0);

// Symmetric (can be overridden by individual extensions) update preferences.
// e.g.
//  extensions.{GUID}.update.enabled
//  extensions.{GUID}.update.url
//  extensions.{GUID}.update.interval
//  .. etc ..
//
pref("extensions.update.enabled", true);
pref("extensions.update.url", "https://add-ons.drood.com/update/VersionCheck.php?reqVersion=%REQ_VERSION%&id=%ITEM_ID%&version=%ITEM_VERSION%&maxAppVersion=%ITEM_MAXAPPVERSION%&status=%ITEM_STATUS%&appID=%APP_ID%&appVersion=%APP_VERSION%&appOS=%APP_OS%&appABI=%APP_ABI%&locale=%APP_LOCALE%");
pref("extensions.update.interval", 86400);  // Check for updates to Extensions and 
                                            // Themes every day
// Non-symmetric (not shared by extensions) extension-specific [update] preferences
// pref("extensions.getMoreExtensionsURL", "https://%LOCALE%.add-ons.drood.com/%LOCALE%/%APP%/%VERSION%/extensions/");
// pref("extensions.getMoreThemesURL", "https://%LOCALE%.add-ons.drood.com/%LOCALE%/%APP%/%VERSION%/themes/");
// pref("extensions.getMorePluginsURL", "https://%LOCALE%.add-ons.drood.com/%LOCALE%/%APP%/%VERSION%/plugins/");
pref("extensions.getMoreExtensionsURL", "http://www.drood.com/toolbox.html");
pref("extensions.getMoreThemesURL", "http://www.drood.com/toolbox.html");
pref("extensions.getMorePluginsURL", "http://www.drood.com/toolbox.html");
pref("extensions.dss.enabled", false);          // Dynamic Skin Switching                                               
pref("extensions.dss.switchPending", false);    // Non-dynamic switch pending after next
                                                // restart.

pref("xpinstall.whitelist.add", "studio.drood.com");
pref("xpinstall.whitelist.add.103", "studio.drood.com");

pref("general.useragent.locale", "@AB_CD@");
pref("general.skins.selectedSkin", "classic/1.0");
pref("general.useragent.extra.drood", "@APP_UA_NAME@/@APP_VERSION@");

pref("general.smoothScroll", false);
#ifdef UNIX_BUT_NOT_MAC
pref("general.autoScroll", false);
#else
pref("general.autoScroll", true);
#endif

// Scripts & Windows prefs
pref("dom.disable_open_during_load",              true);
pref("javascript.options.showInConsole",          false);
// Make the status bar reliably present and unaffected by pages
pref("dom.disable_window_open_feature.status",    true);
// This is the pref to control the location bar, change this to true to 
// force this instead of or in addition to the status bar - this makes 
// the origin of popup windows more obvious to avoid spoofing but we 
// cannot do it by default because it affects UE for web applications.
pref("dom.disable_window_open_feature.location",  false);
pref("dom.disable_window_status_change",          true);
// allow JS to move and resize existing windows
pref("dom.disable_window_move_resize",            false);
// prevent JS from monkeying with window focus, etc
pref("dom.disable_window_flip",                   true);
 
pref("privacy.item.history",    true);
pref("privacy.item.formdata",   true);
pref("privacy.item.passwords",  false);
pref("privacy.item.downloads",  true);
pref("privacy.item.cookies",    false);
pref("privacy.item.cache",      true);
pref("privacy.item.siteprefs",  false);
pref("privacy.item.sessions",   true);

pref("privacy.sanitize.sanitizeOnShutdown", false);
pref("privacy.sanitize.promptOnSanitize", true);

pref("network.proxy.share_proxy_settings",  false); // use the same proxy settings for all protocols

pref("network.cookie.cookieBehavior",       0); // cookies enabled
pref("network.cookie.enableForCurrentSessionOnly", false);

// l12n and i18n
pref("intl.accept_languages", "chrome://global/locale/intl.properties");
// collationOption is only set on linux for japanese. see bug 18338 and 62015
// we need to check if this pref is still useful.
pref("intl.collationOption",  "chrome://global-platform/locale/intl.properties");
pref("intl.charset.detector", "chrome://global/locale/intl.properties");
pref("intl.charset.default",  "chrome://global-platform/locale/intl.properties");
pref("font.language.group", "chrome://global/locale/intl.properties");
pref("intl.menuitems.alwaysappendaccesskeys","chrome://global/locale/intl.properties");
pref("intl.menuitems.insertseparatorbeforeaccesskeys","chrome://global/locale/intl.properties");

// 0=lines, 1=pages, 2=history , 3=text size
#ifdef XP_MACOSX
// On OS X, if the wheel has one axis only, shift+wheel comes through as a
// horizontal scroll event. Thus, we can't assign anything other than normal
// scrolling to shift+wheel.
pref("mousewheel.withshiftkey.action",0);
pref("mousewheel.withshiftkey.sysnumlines",true);
pref("mousewheel.withshiftkey.numlines",1);
pref("mousewheel.withaltkey.action",2);
pref("mousewheel.withaltkey.sysnumlines",false);
pref("mousewheel.withaltkey.numlines",1);
pref("mousewheel.withmetakey.action",0);
pref("mousewheel.withmetakey.sysnumlines",false);
pref("mousewheel.withmetakey.numlines",1);
#else
pref("mousewheel.withshiftkey.action",2);
pref("mousewheel.withshiftkey.sysnumlines",false);
pref("mousewheel.withshiftkey.numlines",1);
pref("mousewheel.withaltkey.action",0);
pref("mousewheel.withaltkey.sysnumlines",false);
pref("mousewheel.withaltkey.numlines",1);
pref("mousewheel.withmetakey.action",0);
pref("mousewheel.withmetakey.sysnumlines",true);
pref("mousewheel.withmetakey.numlines",1);
#endif
pref("mousewheel.withcontrolkey.action",3);
pref("mousewheel.withcontrolkey.sysnumlines",false);
pref("mousewheel.withcontrolkey.numlines",1);

pref("profile.allow_automigration", false);   // setting to false bypasses automigration in the profile code

// Customizable toolbar stuff
pref("custtoolbar.personal_toolbar_folder", "");

// pref to control the alert notification 
pref("alerts.slideIncrement", 1);
pref("alerts.slideIncrementTime", 10);
pref("alerts.totalOpenTime", 4000);
pref("alerts.height", 50);

pref("browser.xul.error_pages.enabled", true);

pref("signon.rememberSignons",              true);
pref("signon.expireMasterPassword",         false);
pref("signon.SignonFileName", "signons.txt");

// We want to make sure mail URLs are handled externally...
pref("network.protocol-handler.external.mailto", true); // for mail
pref("network.protocol-handler.external.news", true);   // for news
pref("network.protocol-handler.external.snews", true);  // for secure news
pref("network.protocol-handler.external.nntp", true);   // also news
// ...without warning dialogs
pref("network.protocol-handler.warn-external.mailto", false);
pref("network.protocol-handler.warn-external.news", false);
pref("network.protocol-handler.warn-external.snews", false);
pref("network.protocol-handler.warn-external.nntp", false);
pref("network.protocol-handler.warn-external.http", false);
pref("network.protocol-handler.warn-external.https", false);
pref("network.protocol-handler.warn-external.ftp", false);
pref("network.protocol-handler.warn-external.file", false);

// By default, all protocol handlers are exposed.  This means that
// the browser will respond to openURL commands for all URL types.
// It will also try to open link clicks inside the browser before
// failing over to the system handlers.
pref("network.protocol-handler.expose-all", true);
pref("network.protocol-handler.expose.mailto", false);
pref("network.protocol-handler.expose.news", false);
pref("network.protocol-handler.expose.snews", false);
pref("network.protocol-handler.expose.nntp", false);
pref("network.protocol-handler.expose.http", true);
pref("network.protocol-handler.expose.https", true);
pref("network.protocol-handler.expose.ftp", false);
pref("network.protocol-handler.expose.drood", true);

// Default security warning dialogs to show once.
pref("security.warn_entering_secure", false);
pref("security.warn_entering_secure.show_once", true);
pref("security.warn_entering_weak.show_once", true);
pref("security.warn_leaving_secure.show_once", true);
pref("security.warn_viewing_mixed.show_once", true);
pref("security.warn_submit_insecure.show_once", true);

pref("accessibility.typeaheadfind", false);
pref("accessibility.typeaheadfind.timeout", 5000);
pref("accessibility.typeaheadfind.linksonly", false);
pref("accessibility.typeaheadfind.flashBar", 1);

// Disable the default plugin for Drood
pref("plugin.default_plugin_disabled", true);

// plugin finder service url
pref("pfs.datasource.url", "https://angstrom.greyfirst.net/plugins/PluginFinderService.php?mimetype=%PLUGIN_MIMETYPE%&appID=%APP_ID%&appVersion=%APP_VERSION%&clientOS=%CLIENT_OS%&chromeLocale=%CHROME_LOCALE%");

// This is really a global preference, since it affects the prefwindow widget
pref("browser.preferences.instantApply", false);
#ifdef XP_MACOSX
pref("browser.preferences.animateFadeIn", true);
#else
pref("browser.preferences.animateFadeIn", false);
#endif

// Setting this pref to |true| forces BiDi UI menu items and keyboard shortcuts
// to be exposed. By default, only expose it for bidi-associated system locales.
pref("bidi.browser.ui", false);

// this will automatically enable inline spellchecking (if it is available) for
// editable elements in HTML
// 0 = spellcheck nothing
// 1 = check multi-line controls [default]
// 2 = check multi/single line controls
pref("layout.spellcheckDefault", 0);

// Spellbound preferences
pref("spellbound.editor.display.background_color", "#FFFFFF");
pref("spellbound.editor.display.foreground_color", "#000000");
pref("spellbound.editor.display.wrap", true);
pref("spellbound.editor.height", 160);
pref("spellbound.editor.misspelled.display.background_color", "#FFFFFF");
pref("spellbound.editor.misspelled.display.bold", false);
pref("spellbound.editor.misspelled.display.foreground_color", "#FF0000");
pref("spellbound.editor.misspelled.display.italic", false);
pref("spellbound.editor.misspelled.display.underline", true);
pref("spellbound.editor.misspelled.enabled", true);
pref("spellbound.spellcheck.modal", true);
pref("spellbound.toolsmenuitem.hidden", true);

// General stuff
pref("drood.toolbar.show", "both");
pref("drood.spelling.inline", true);
pref("drood.user.loginOnStartup", false);
pref("drood.inbox.refreshRate", -1);
pref("drood.autosave", -1);
pref("drood.superbundle.installing", false);

// Server prefs
pref("drood.server.ping", true);
pref("drood.server.promptForCommitMessage", true);
pref("drood.server.publish.selection", "publish.drood.com");
pref("drood.server.publish.list", "publish.drood.com");
pref("drood.server.render.selection", "render.drood.com");
pref("drood.server.render.list", "render.drood.com");
pref("drood.server.studio.scheme", "https");
pref("drood.server.studio.selection", "www.drood.com");
pref("drood.server.studio.list", "www.drood.com=DroodServices,studio.drood.com=LegacyStudio");

// New project defaults
pref("drood.newproject.script", true);
pref("drood.newproject.stageplay", false);
pref("drood.newproject.text", false);
pref("drood.newproject.storyboard", true);
pref("drood.newproject.location", false);
pref("drood.newproject.scene", false);
pref("drood.newproject.actor", false);
pref("drood.newproject.character", false);
pref("drood.newproject.wardrobe", false);
pref("drood.newproject.props", false);
pref("drood.newproject.schedule", false);

// Drood PDF Options
pref("drood.pdf.useraccepted", false);
pref("drood.pdf.audio.format", "default");
pref("drood.pdf.dialog.breaks.enabled", true);
pref("drood.pdf.dialog.breakbottom.enabled", true);
pref("drood.pdf.dialog.breakbottom.text", "(MORE)");
pref("drood.pdf.dialog.breaktop.enabled", true);
pref("drood.pdf.dialog.breaktop.text", "(cont'd)");
pref("drood.pdf.dialog.autocharcontinueds", false);
pref("drood.pdf.scene.breaks.enabled", true);
pref("drood.pdf.scene.breakbottom.enabled", false);
pref("drood.pdf.scene.breakbottom.text", "(CONTINUED)");
pref("drood.pdf.scene.breaktop.enabled", false);
pref("drood.pdf.scene.breaktop.text", "CONTINUED:");
pref("drood.pdf.scene.continuedsnumbered", false);
pref("drood.pdf.scene.pagebreaks", false);

// How many lines constitute a page
pref("drood.script.papersize", "USLetter");
pref("drood.script.USLetter.lines", 52);
pref("drood.script.A4.lines", 57);

// Whether breakdown is visible in the script
pref("drood.script.breakdown.visible", true);

// Film Script Specifications

pref("drood.scripteditor.film.formats",
  "Sceneheading,Action,Character,Dialog,Parenthetical,Transition,Shot,Text");

// Auto-complete settings
pref("drood.scripteditor.autocomplete.film.sceneheading", true);
pref("drood.scripteditor.autocomplete.film.shot", true);
pref("drood.scripteditor.autocomplete.film.character", true);
pref("drood.scripteditor.autocomplete.film.sound", false);

// Format dialog as parenthetical when "(" is typed at the beginning
pref("drood.scripteditor.film.autoparenthesize", true);

// Initial state
pref("drood.scripteditor.film.default", "sceneheading");

// Default Transitions
pref("drood.scripteditor.film.tab.default", "action");
pref("drood.scripteditor.film.shifttab.default", "action");
pref("drood.scripteditor.film.enter.default", "action");
pref("drood.scripteditor.film.blankenter.default", "action");

// Transitions from Scene Heading
pref("drood.scripteditor.film.tab.sceneheading", "action");
pref("drood.scripteditor.film.enter.sceneheading", "action");
pref("drood.scripteditor.film.blankenter.sceneheading", "action");

// Transitions from Action
pref("drood.scripteditor.film.tab.action", "character");
pref("drood.scripteditor.film.shifttab.action", "sceneheading");
pref("drood.scripteditor.film.enter.action", "action");
pref("drood.scripteditor.film.blankenter.action", "sceneheading");

// Transitions from Character
pref("drood.scripteditor.film.shifttab.character", "action");
pref("drood.scripteditor.film.enter.character", "dialog");
pref("drood.scripteditor.film.blankenter.character", "action");

// Transitions from Dialog
pref("drood.scripteditor.film.tab.dialog", "parenthetical");
pref("drood.scripteditor.film.enter.dialog", "character");
pref("drood.scripteditor.film.blankenter.dialog", "action");

// Transitions from Parenthetical
pref("drood.scripteditor.film.tab.parenthetical", "dialog");
pref("drood.scripteditor.film.shifttab.parenthetical", "dialog");
pref("drood.scripteditor.film.enter.parenthetical", "dialog");
pref("drood.scripteditor.film.blankenter.parenthetical", "dialog");

// Transitions from Transition (haha)
pref("drood.scripteditor.film.enter.transition", "sceneheading");
pref("drood.scripteditor.film.blankenter.transition", "action");

// Transitions from Text
pref("drood.scripteditor.film.enter.text", "text");
pref("drood.scripteditor.film.blankenter.text", "text");


// Theatrical Script Specifications

pref("drood.scripteditor.theatre.formats",
  "Act,Sceneheading,StageDir,Character,Dialog,Parenthetical,Transition,Text");

// Auto-complete settings
pref("drood.scripteditor.autocomplete.theatre.sceneheading", false);
pref("drood.scripteditor.autocomplete.theatre.shot", false);
pref("drood.scripteditor.autocomplete.theatre.character", true);
pref("drood.scripteditor.autocomplete.theatre.sound", false);

// Format dialog as parenthetical when "(" is typed at the beginning
pref("drood.scripteditor.theatre.autoparenthesize", true);

// Initial state
pref("drood.scripteditor.theatre.default", "act");

// Default Transitions
pref("drood.scripteditor.theatre.tab.default", "action");
pref("drood.scripteditor.theatre.shifttab.default", "character");
pref("drood.scripteditor.theatre.enter.default", "action");
pref("drood.scripteditor.theatre.blankenter.default", "action");

// Transitions from Act
pref("drood.scripteditor.theatre.enter.act", "sceneheading");
pref("drood.scripteditor.theatre.blankenter.act", "act");

// Transitions from Sceneheading
pref("drood.scripteditor.theatre.tab.sceneheading", "action");
pref("drood.scripteditor.theatre.shifttab.sceneheading", "character");
pref("drood.scripteditor.theatre.enter.sceneheading", "action");
pref("drood.scripteditor.theatre.blankenter.sceneheading", "action");

// Transitions from Action
pref("drood.scripteditor.theatre.tab.action", "character");
pref("drood.scripteditor.theatre.enter.action", "action");
pref("drood.scripteditor.theatre.blankenter.action", "character");

// Transitions from Character
pref("drood.scripteditor.theatre.tab.character", "parenthetical");
pref("drood.scripteditor.theatre.enter.character", "dialog");
pref("drood.scripteditor.theatre.blankenter.character", "action");

// Transitions from Dialog
pref("drood.scripteditor.theatre.tab.dialog", "parenthetical");
pref("drood.scripteditor.theatre.enter.dialog", "character");
pref("drood.scripteditor.theatre.blankenter.dialog", "action");

// Transitions from Parenthetical
pref("drood.scripteditor.theatre.tab.parenthetical", "dialog");
pref("drood.scripteditor.theatre.enter.parenthetical", "dialog");
pref("drood.scripteditor.theatre.blankenter.parenthetical", "dialog");

// A/V Script Specifications

pref("drood.scripteditor.av.formats",
  "Sceneheading,Shot,Character,Dialog,Parenthetical");

// Auto-complete settings
pref("drood.scripteditor.autocomplete.av.sceneheading", true);
pref("drood.scripteditor.autocomplete.av.shot", true);
pref("drood.scripteditor.autocomplete.av.character", true);
pref("drood.scripteditor.autocomplete.av.sound", false);

// Format dialog as parenthetical when "(" is typed at the beginning
pref("drood.scripteditor.av.autoparenthesize", true);

// Initial state
pref("drood.scripteditor.av.default", "sceneheading");

// Default transitions (also text transitions)
pref("drood.scripteditor.av.tab.default", "shot");
pref("drood.scripteditor.av.shifttab.default", "sceneheading");
pref("drood.scripteditor.av.enter.default", "shot");
pref("drood.scripteditor.av.blankenter.default", "shot");

// Sequence transitions
pref("drood.scripteditor.av.tab.sceneheading", "shot");
pref("drood.scripteditor.av.shifttab.sceneheading", "dialog");
pref("drood.scripteditor.av.enter.sceneheading", "shot");
pref("drood.scripteditor.av.blankenter.sceneheading", "shot");

// Shot transitions
pref("drood.scripteditor.av.tab.shot", "character");
pref("drood.scripteditor.av.shifttab.shot", "sceneheading");
pref("drood.scripteditor.av.enter.shot", "character");
pref("drood.scripteditor.av.blankenter.shot", "sceneheading");

// Character transitions
pref("drood.scripteditor.av.tab.character", "parenthetical");
pref("drood.scripteditor.av.shifttab.character", "shot");
pref("drood.scripteditor.av.enter.character", "dialog");
pref("drood.scripteditor.av.blankenter.character", "shot");

// Dialog transitions
pref("drood.scripteditor.av.tab.dialog", "sceneheading");
pref("drood.scripteditor.av.shifttab.dialog", "parenthetical");
pref("drood.scripteditor.av.enter.dialog", "character");
pref("drood.scripteditor.av.blankenter.dialog", "shot");

// Parenthetical transitions
pref("drood.scripteditor.av.tab.parenthetical", "dialog");
pref("drood.scripteditor.av.shifttab.parenthetical", "character");
pref("drood.scripteditor.av.enter.parenthetical", "dialog");
pref("drood.scripteditor.av.blankenter.parenthetical", "dialog");

// Radio Script Specifications

pref("drood.scripteditor.radio.formats",
  "Sceneheading,ProductionNote,Character,Dialog,Parenthetical,Sound,Voice,Music");

// Auto-complete settings
pref("drood.scripteditor.autocomplete.radio.sceneheading", true);
pref("drood.scripteditor.autocomplete.radio.shot", false);
pref("drood.scripteditor.autocomplete.radio.character", true);
pref("drood.scripteditor.autocomplete.radio.sound", true);

// Format dialog as parenthetical when "(" is typed at the beginning
pref("drood.scripteditor.radio.autoparenthesize", true);

// Initial state
pref("drood.scripteditor.radio.default", "sceneheading");

// Default transitions
pref("drood.scripteditor.radio.tab.default", "character");
pref("drood.scripteditor.radio.shifttab.default", "character");
pref("drood.scripteditor.radio.enter.default", "character");
pref("drood.scripteditor.radio.blankenter.default", "character");

// Scene transitions
pref("drood.scripteditor.radio.tab.sceneheading", "action");
pref("drood.scripteditor.radio.shifttab.sceneheading", "music");
pref("drood.scripteditor.radio.enter.sceneheading", "action");
pref("drood.scripteditor.radio.blankenter.sceneheading", "character");

// Production Note transitions
pref("drood.scripteditor.radio.tab.action", "character");
pref("drood.scripteditor.radio.shifttab.action", "sceneheading");
pref("drood.scripteditor.radio.enter.action", "character");
pref("drood.scripteditor.radio.blankenter.action", "character");

// Character transitions
pref("drood.scripteditor.radio.tab.character", "parenthetical");
pref("drood.scripteditor.radio.shifttab.character", "action");
pref("drood.scripteditor.radio.enter.character", "dialog");
pref("drood.scripteditor.radio.blankenter.character", "sound");

// Dialog transitions
pref("drood.scripteditor.radio.tab.dialog", "sound");
pref("drood.scripteditor.radio.shifttab.dialog", "parenthetical");
pref("drood.scripteditor.radio.enter.dialog", "character");
pref("drood.scripteditor.radio.blankenter.dialog", "character");

// Parenthetical transitions
pref("drood.scripteditor.radio.tab.parenthetical", "dialog");
pref("drood.scripteditor.radio.shifttab.parenthetical", "character");
pref("drood.scripteditor.radio.enter.parenthetical", "dialog");
pref("drood.scripteditor.radio.blankenter.parenthetical", "dialog");

// Sound transitions
pref("drood.scripteditor.radio.tab.sound", "voice");
pref("drood.scripteditor.radio.shifttab.sound", "dialog");
pref("drood.scripteditor.radio.enter.sound", "character");
pref("drood.scripteditor.radio.blankenter.sound", "character");

// Voice transitions
pref("drood.scripteditor.radio.tab.voice", "music");
pref("drood.scripteditor.radio.shifttab.voice", "sound");
pref("drood.scripteditor.radio.enter.voice", "character");
pref("drood.scripteditor.radio.blankenter.voice", "character");

// Music transitions
pref("drood.scripteditor.radio.tab.music", "sceneheading");
pref("drood.scripteditor.radio.shifttab.music", "voice");
pref("drood.scripteditor.radio.enter.music", "character");
pref("drood.scripteditor.radio.blankenter.music", "character");



// Comic Script Specifications

pref("drood.scripteditor.comic.formats",
  "Page,Panel,Caption,Character,BalloonType,Balloon");

// Auto-complete settings
pref("drood.scripteditor.autocomplete.comic.sceneheading", false);
pref("drood.scripteditor.autocomplete.comic.shot", false);
pref("drood.scripteditor.autocomplete.comic.character", true);
pref("drood.scripteditor.autocomplete.comic.sound", false);

// Format dialog as parenthetical when "(" is typed at the beginning
pref("drood.scripteditor.comic.autoparenthesize", true);

// Initial state
pref("drood.scripteditor.comic.default", "sceneheading");

// Default transitions
pref("drood.scripteditor.comic.tab.default", "character");
pref("drood.scripteditor.comic.shifttab.default", "character");
pref("drood.scripteditor.comic.enter.default", "character");
pref("drood.scripteditor.comic.blankenter.default", "character");

// Scene transitions
pref("drood.scripteditor.comic.tab.sceneheading", "shot");
pref("drood.scripteditor.comic.shifttab.sceneheading", "dialog");
pref("drood.scripteditor.comic.enter.sceneheading", "shot");
// use enter rule for blankenter

// Shot transitions
pref("drood.scripteditor.comic.tab.shot", "caption");
pref("drood.scripteditor.comic.shifttab.shot", "sceneheading");
pref("drood.scripteditor.comic.enter.shot", "caption");
pref("drood.scripteditor.comic.blankenter.shot", "sceneheading");

// Caption transitions
pref("drood.scripteditor.comic.tab.caption", "character");
pref("drood.scripteditor.comic.shifttab.caption", "shot");
pref("drood.scripteditor.comic.enter.caption", "character");
pref("drood.scripteditor.comic.blankenter.caption", "character");

// Character transitions
pref("drood.scripteditor.comic.tab.character", "shot");
pref("drood.scripteditor.comic.shifttab.character", "caption");
pref("drood.scripteditor.comic.enter.character", "dialog");
pref("drood.scripteditor.comic.blankenter.character", "shot");

// Parenthetical transitions
pref("drood.scripteditor.comic.tab.parenthetical", "dialog");
pref("drood.scripteditor.comic.shifttab.parenthetical", "character");
pref("drood.scripteditor.comic.enter.parenthetical", "dialog");
pref("drood.scripteditor.comic.blankenter.parenthetical", "dialog");

// Dialog transitions
pref("drood.scripteditor.comic.tab.dialog", "parenthetical");
pref("drood.scripteditor.comic.shifttab.dialog", "character");
pref("drood.scripteditor.comic.enter.dialog", "character");
pref("drood.scripteditor.comic.blankenter.dialog", "shot");


pref("drood.scenecards.showtags", true);

pref("drood.texteditor.zoomLevel", 100);

pref("drood.sketch.font.name", "Times");
pref("drood.sketch.font.size", 16);


// Scheduling preferences
pref("calendar.view.daystarthour", 8);
pref("calendar.view.dayendhour", 17);
pref("drood.calendar.scene.defaultlength", 120);
pref("drood.calendar.workdaysonly", false);

pref("drood.storyboard.slideshow.delay", 2);
pref("drood.storyboard.print.orientation", "portrait");

pref("browser.preferences.animateFadeIn", false);
pref("calendar.week.start", 1);
pref("calendar.week.d0sundaysoff", false);
pref("calendar.week.d1mondaysoff", false);
pref("calendar.week.d2tuesdaysoff", false);
pref("calendar.week.d3wednesdaysoff", false);
pref("calendar.week.d4thursdaysoff", false);
pref("calendar.week.d5fridaysoff", false);
pref("calendar.week.d6saturdaysoff", false);

// No reason to spook the user
pref("security.warn_submit_insecure", false);
pref("security.warn_submit_insecure.show_once", false);

// InstantBird default preferences
pref("messenger.accounts", "");
pref("messenger.accounts.promptOnDelete", true);

// The intervals in seconds between automatic reconnection attempts
// The last value will be reused forever.
// A value of 0 means that there will be no more reconnection attempts.
pref("messenger.accounts.reconnectTimer", "1,5,30,60,90,300,600,1200,3600");


pref("messenger.conversations.selections.ellipsis", "chrome://instantbird/locale/instantbird.properties");
pref("messenger.conversations.selections.systemMessagesTemplate", "chrome://instantbird/locale/instantbird.properties");
pref("messenger.conversations.selections.contentMessagesTemplate", "chrome://instantbird/locale/instantbird.properties");
pref("messenger.conversations.selections.actionMessagesTemplate", "chrome://instantbird/locale/instantbird.properties");

pref("messenger.options.playSounds", false);

// this preference changes how we filter incoming messages
// 0 = no formattings
// 1 = basic formattings (bold, italic, underlined)
// 2 = permissive mode (colors, font face, font size, ...)
pref("messenger.options.filterMode", 1);

// use "none" to disable
pref("messenger.options.emoticonsTheme", "default");
pref("messenger.options.messagesStyle.theme", "minimal20");
pref("messenger.options.messagesStyle.variant", "default");
pref("messenger.options.messagesStyle.showHeader", false);
pref("messenger.options.messagesStyle.combineConsecutive", true);
// if the time interval in seconds between two messages is longer than
// this value, the messages will not be combined
pref("messenger.options.messagesStyle.combineConsecutiveInterval", 300); // 5 minutes

pref("messenger.proxies", "");
pref("messenger.globalProxy", "none");
