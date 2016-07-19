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

#ifndef DROOD_RDF_UTILS_H_
#define DROOD_RDF_UTILS_H_

#include "nsIRDFDataSource.h"
#include "nsIRDFResource.h"
#include "nsIRDFContainer.h"

class DroodRDFUtils {
public:
  static nsresult GetRDFSeq (nsIRDFDataSource* aDS,
                             nsIRDFResource* aRes,
                             nsIRDFContainer** result);
};

#endif /* DROOD_RDF_UTILS_H_ */
