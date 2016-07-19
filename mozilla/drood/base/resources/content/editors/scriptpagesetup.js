/* ***** BEGIN LICENCE BLOCK *****
 * Version: MPL 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 * 
 * The Original Code is Drood Script Manager.
 * 
 * The Initial Developer of the Original Code is 4067479 Canada Inc.
 * t/a DROOD.
 * 
 * Portions created by Drood are Copyright (C) 4067479 Canada Inc. All
 * Rights Reserved.
 * 
 * Contributor(s):
 *
 ***** END LICENCE BLOCK ***** */

var gDialog;
var gDroodSettings;

function loaded() {
  gDialog = new Object;

  gDialog.pageNumbers     = document.getElementById("pageNumbers");
  gDialog.firstPageNumber = document.getElementById("firstPageNumber");
  gDialog.sceneNumbers    = document.getElementById("sceneNumbers");
  gDialog.paperSize       = document.getElementById("paperSize");
  gDialog.dialogNumbers   = document.getElementById("dialogNumbers");
  gDialog.charNumbers     = document.getElementById("charNumbers");
  gDialog.linesPerPage    = document.getElementById("linesPerPage");

  initSettings();
}

function initSettings() {
  gDroodSettings = window.arguments[0];

  gDialog.paperSize.selectedItem =
    getItemByValue(gDialog.paperSize, gDroodSettings.paperSize);

  gDialog.pageNumbers.checked       = gDroodSettings.pageNumbers;
  gDialog.firstPageNumber.checked   = gDroodSettings.firstPageNumber;
  gDialog.firstPageNumber.disabled  = ! gDroodSettings.pageNumbers;
  gDialog.linesPerPage.disabled     = ! gDroodSettings.pageNumbers;

  var ps = getPrefService().getBranch("drood.script.");
  gDialog.linesPerPage.value = ps.getIntPref(
    gDialog.paperSize.value + ".lines");

  if (gDroodSettings.sceneNumbers)
    gDialog.sceneNumbers.selectedItem =
      getItemByValue(gDialog.sceneNumbers, gDroodSettings.sceneNumbers);
  gDialog.sceneNumbers.disabled     = gDroodSettings.disableSceneNumbers;

  if (gDroodSettings.mode == "film") {
    gDialog.dialogNumbers.hidden = false;
    gDialog.dialogNumbers.checked = gDroodSettings.dialogNumbers;
  }
  else if (gDroodSettings.mode == "radio") {
    gDialog.charNumbers.hidden = false;
    gDialog.charNumbers.checked = gDroodSettings.charNumbers;
  }

  if (gDroodSettings.hideSceneNumbers)
    document.getElementById("sceneNumberBox").collapsed = true;
  if (gDroodSettings.hidePageNumbers)
    document.getElementById("pageNumberBox").collapsed = true;
}

function paperSizeChanged () {
  var ps = getPrefService().getBranch("drood.script.");
  gDialog.linesPerPage.value = ps.getIntPref(
    gDialog.paperSize.value + ".lines");
}

function accepted (event) {
  var linesPerPage = Number(gDialog.linesPerPage.value);
  if (isNaN(linesPerPage) || linesPerPage < 40 || linesPerPage > 80) {
    gDialog.linesPerPage.value = gApp.getText("ValidLinesPerPage");
    if (event)
      event.preventDefault();
    return false;
  }
  gDroodSettings.accepted         = true;
  gDroodSettings.paperSize        = gDialog.paperSize.selectedItem.value;
  gDroodSettings.pageNumbers      = gDialog.pageNumbers.checked;
  gDroodSettings.firstPageNumber  = gDialog.firstPageNumber.checked;
  gDroodSettings.sceneNumbers     = gDialog.sceneNumbers.selectedItem.value;

  var ps = getPrefService().getBranch("drood.script.");
  ps.setIntPref(gDialog.paperSize.value + ".lines", linesPerPage);

  if (gDroodSettings.mode == "film")
    gDroodSettings.dialogNumbers = gDialog.dialogNumbers.checked;
  else if (gDroodSettings.mode == "radio")
    gDroodSettings.charNumbers = gDialog.charNumbers.checked;

  return true;
}

function canceled() {
  return true;
}
