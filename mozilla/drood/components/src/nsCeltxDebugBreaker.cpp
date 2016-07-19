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

#include "nsDroodDebugBreaker.h"
#include "nsPIDOMWindow.h"
#include "nsIDocShell.h"
#include "nsIBaseWindow.h"
#include "nsIWidget.h"
#include "nsGUIEvent.h"
#include <stdio.h>

NS_IMPL_ISUPPORTS1(nsDroodDebugBreaker, nsIDroodDebugBreaker)

nsDroodDebugBreaker::nsDroodDebugBreaker () {}

nsDroodDebugBreaker::~nsDroodDebugBreaker () {}

/* void breakToDebug (); */
NS_IMETHODIMP nsDroodDebugBreaker::BreakToDebug () {
  printf("nsDroodDebugBreaker::BreakToDebug called\n");
  return NS_OK;
}

/* void fixWindowFocus (); */
NS_IMETHODIMP nsDroodDebugBreaker::FixWindowFocus (nsIDOMWindow* aWindow) {
  NS_ENSURE_ARG_POINTER(aWindow);
  printf("nsDroodDebugBreaker::FixWindowFocus called\n");
  nsCOMPtr<nsPIDOMWindow> pwin = do_QueryInterface(aWindow);
  if (! pwin) {
    printf("*** failed to QI window to nsPIDOMWindow\n");
    return NS_OK;
  }
  nsCOMPtr<nsIBaseWindow> bwin = do_QueryInterface(pwin->GetDocShell());
  if (! bwin) {
    printf("*** failed to QI docshell to nsIBaseWindow\n");
    return NS_OK;
  }
  nsCOMPtr<nsIWidget> mainWidget;
  nsresult rv = bwin->GetMainWidget(getter_AddRefs(mainWidget));
  NS_ENSURE_SUCCESS(rv, rv);

  nsGUIEvent focusEvent(PR_TRUE, NS_GOTFOCUS, mainWidget);
  focusEvent.time = PR_IntervalNow();
  nsEventStatus status = nsEventStatus_eIgnore;
  return mainWidget->DispatchEvent(&focusEvent, status);
}
