/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is JavaScript Engine testing utilities.
 *
 * The Initial Developer of the Original Code is
 * Mozilla Foundation.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): Mike Shaver
 *                 Brendan Eich
 *                 Andreas Gal
 *                 David Anderson
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

var gTestfile = 'trace-test.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 'none';
var summary = 'trace-capability mini-testsuite';

printBugNumber(BUGNUMBER);
printStatus (summary);
 
var testName = null;
if ("arguments" in this && arguments.length > 0)
  testName = arguments[0];
var fails = [], passes=[];

function test(f)
{
  if (!testName || testName == f.name)
    check(f.name, f(), f.expected);
}

function check(desc, actual, expected)
{
  if (expected == actual) {
    reportCompare(expected + '', actual + '', desc);
    passes.push(desc);
    return print(desc, ": passed");
  }
  reportCompare(expected, actual, desc);
  fails.push(desc);
  print(desc, ": FAILED: expected", typeof(expected), "(", expected, ") != actual",
	typeof(actual), "(", actual, ")");
}

function ifInsideLoop()
{
  var cond = true, intCond = 5, count = 0;
  for (var i = 0; i < 100; i++) {
    if (cond)
      count++;
    if (intCond)
      count++;
  }
  return count;
}
ifInsideLoop.expected = 200;
test(ifInsideLoop);

function bitwiseAnd_inner(bitwiseAndValue) {
  for (var i = 0; i < 60000; i++)
    bitwiseAndValue = bitwiseAndValue & i;
  return bitwiseAndValue;
}
function bitwiseAnd()
{
  return bitwiseAnd_inner(12341234);
}
bitwiseAnd.expected = 0;
test(bitwiseAnd);

if (!testName || testName == "bitwiseGlobal") {
  bitwiseAndValue = Math.pow(2,32);
  for (var i = 0; i < 60000; i++)
    bitwiseAndValue = bitwiseAndValue & i;
  check("bitwiseGlobal", bitwiseAndValue, 0);
}


function equalInt()
{
  var i1 = 55;
  var hits = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
  for (var i = 0; i < 5000; i++) {
    if (i1 == 55) hits[0]++;
    if (i1 != 56) hits[1]++;
    if (i1 < 56)  hits[2]++;
    if (i1 > 50)  hits[3]++;
    if (i1 <= 60) hits[4]++;
    if (i1 >= 30) hits[5]++;
    if (i1 == 7)  hits[6]++;
    if (i1 != 55) hits[7]++;
    if (i1 < 30)  hits[8]++;
    if (i1 > 90)  hits[9]++;
    if (i1 <= 40) hits[10]++;
    if (i1 >= 70) hits[11]++;
  }
  return hits.toString();
}
equalInt.expected = "5000,5000,5000,5000,5000,5000,0,0,0,0,0,0,0,0,0,0,0,0,0";
test(equalInt);

var a;
function setelem()
{
  a = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
  a = a.concat(a, a, a);
  var l = a.length;
  for (var i = 0; i < l; i++) {
    a[i] = i;
  }
  return a.toString();
}
setelem.expected = "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83";
test(setelem);

function getelem_inner(a)
{
  var accum = 0;
  var l = a.length;
  for (var i = 0; i < l; i++) {
    accum += a[i];
  }
  return accum;
}
function getelem()
{
  return getelem_inner(a);
}
getelem.expected = 3486;
test(getelem);

globalName = 907;
function name()
{
  var a = 0;
  for (var i = 0; i < 100; i++)
    a = globalName;
  return a;
}
name.expected = 907;
test(name);

var globalInt = 0;
if (!testName || testName == "globalGet") {
  for (var i = 0; i < 500; i++)
    globalInt = globalName + i;
  check("globalGet", globalInt, globalName + 499);
}

if (!testName || testName == "globalSet") {
  for (var i = 0; i < 500; i++)
    globalInt = i;
  check("globalSet", globalInt, 499);
}

function arith()
{
  var accum = 0;
  for (var i = 0; i < 100; i++) {
    accum += (i * 2) - 1;
  }
  return accum;
}
arith.expected = 9800;
test(arith);

function lsh_inner(n)
{
  var r;
  for (var i = 0; i < 35; i++)
    r = 0x1 << n;
  return r;
}
function lsh()
{
  return [lsh_inner(15),lsh_inner(55),lsh_inner(1),lsh_inner(0)];
}
lsh.expected = "32768,8388608,2,1";
test(lsh);

function rsh_inner(n)
{
  var r;
  for (var i = 0; i < 35; i++)
    r = 0x11010101 >> n;
  return r;
}
function rsh()
{
  return [rsh_inner(8),rsh_inner(5),rsh_inner(35),rsh_inner(-1)];
}
rsh.expected = "1114369,8914952,35659808,0";
test(rsh);

function ursh_inner(n)
{
  var r;
  for (var i = 0; i < 35; i++)
    r = -55 >>> n;
  return r;
}
function ursh() {
  return [ursh_inner(8),ursh_inner(33),ursh_inner(0),ursh_inner(1)];
}
ursh.expected = "16777215,2147483620,4294967241,2147483620";
test(ursh);

function doMath_inner(cos)
{
    var s = 0;
    var sin = Math.sin;
    for (var i = 0; i < 200; i++)
        s = -Math.pow(sin(i) + cos(i * 0.75), 4);
    return s;
}
function doMath() {
  return doMath_inner(Math.cos);
}
doMath.expected = -0.5405549555611059;
test(doMath);

function fannkuch() {
   var count = Array(8);
   var r = 8;
   var done = 0;
   while (done < 40) {
      // write-out the first 30 permutations
      done += r;
      while (r != 1) { count[r - 1] = r; r--; }
      while (true) {
         count[r] = count[r] - 1;
         if (count[r] > 0) break;
         r++;
      }
   }
   return done;
}
fannkuch.expected = 41;
test(fannkuch);

function xprop()
{
  a = 0;
  for (var i = 0; i < 20; i++)
    a += 7;
  return a;
}
xprop.expected = 140;
test(xprop);

var a = 2;
function getprop_inner(o2)
{
  var o = {a:5};
  var t = this;
  var x = 0;
  for (var i = 0; i < 20; i++) {
    t = this;
    x += o.a + o2.a + this.a + t.a;
  }
  return x;
}
function getprop() {
  return getprop_inner({a:9});
}
getprop.expected = 360;
test(getprop);

function mod()
{
  var mods = [-1,-1,-1,-1];
  var a = 9.5, b = -5, c = 42, d = (1/0);
  for (var i = 0; i < 20; i++) {
    mods[0] = a % b;
    mods[1] = b % 1;
    mods[2] = c % d;
    mods[3] = c % a;
    mods[4] = b % 0;
  }
  return mods.toString();
}
mod.expected = "4.5,0,42,4,NaN";
test(mod);

function glob_f1() {
  return 1;
}
function glob_f2() {
  return glob_f1();
}
function call()
{
  var q1 = 0, q2 = 0, q3 = 0, q4 = 0, q5 = 0;
  var o = {};
  function f1() {
      return 1;
  }
  function f2(f) {
      return f();
  }
  o.f = f1;
  for (var i = 0; i < 100; ++i) {
      q1 += f1();
      q2 += f2(f1);
      q3 += glob_f1();
      q4 += o.f();
      q5 += glob_f2();
  }
  var ret = [q1, q2, q3, q4, q5];
  return ret;
}
call.expected =  "100,100,100,100,100";
test(call);

function setprop()
{
  var obj = { a:-1 };
  var obj2 = { b:-1, a:-1 };
  for (var i = 0; i < 20; i++) {
    obj2.b = obj.a = i;
  }
  return [obj.a, obj2.a, obj2.b].toString();
}
setprop.expected =  "19,-1,19";
test(setprop);

function testif() {
	var q = 0;
	for (var i = 0; i < 100; i++) {
		if ((i & 1) == 0)
			q++;
		else
			q--;
	}
    return q;
}
testif.expected = "0";
test(testif);

function testincops(n) {
  var i = 0, o = {p:0}, a = [0];
  n = 100;

  for (i = 0; i < n; i++);
  while (i-- > 0);
  for (i = 0; i < n; ++i);
  while (--i >= 0);

  for (o.p = 0; o.p < n; o.p++);
  while (o.p-- > 0);
  for (o.p = 0; o.p < n; ++o.p);
  while (--o.p >= 0);

  ++i; // set to 0
  for (a[i] = 0; a[i] < n; a[i]++);
  while (a[i]-- > 0);
  for (a[i] = 0; a[i] < n; ++a[i]);
  while (--a[i] >= 0);

  return [++o.p, ++a[i]].toString();
}
testincops.expected = "0,0";
test(testincops);

function trees() {
  var i = 0, o = [0,0,0];
  for (i = 0; i < 100; ++i) {
    if ((i & 1) == 0) o[0]++;
    else if ((i & 2) == 0) o[1]++;
    else o[2]++;
  }
  return o;
}
trees.expected = "50,25,25";
test(trees);

function unboxint() {
    var q = 0;
    var o = [4];
    for (var i = 0; i < 100; ++i)
	q = o[0] << 1;
    return q;
}
unboxint.expected = "8";
test(unboxint);

function strings()
{
  var a = [], b = -1;
  var s = "abcdefghij", s2 = "a";
  var f = "f";
  var c = 0, d = 0, e = 0, g = 0;
  for (var i = 0; i < 10; i++) {
    a[i] = (s.substring(i, i+1) + s[i] + String.fromCharCode(s2.charCodeAt(0) + i)).concat(i);
    if (s[i] == f)
      c++;
    if (s[i] != 'b')
      d++;
    if ("B" > s2)
      g++; // f already used
    if (s2 < "b")
      e++;
    b = s.length;
  }
  return a.toString() + b + c + d + e + g;
}
strings.expected = "aaa0,bbb1,ccc2,ddd3,eee4,fff5,ggg6,hhh7,iii8,jjj91019100";
test(strings);

function stringConvert()
{
  var a = [];
  var s1 = "F", s2 = "1.3", s3 = "5";
  for (var i = 0; i < 10; i++) {
    a[0] = 1 >> s1;
    a[1] = 10 - s2;
    a[2] = 15 * s3;
    a[3] = s3 | 32;
    // a[4] = s2 + 60;
    // a[5] = 9 + s3;
    // a[6] = -s3;
    a[7] = s3 & "7";
    // a[8] = ~s3;
  }
  return a.toString();
}
stringConvert.expected = "1,8.7,75,37,,,,5";
test(stringConvert);

function orTestHelper(a, b, n)
{
  var k = 0;
  for (var i = 0; i < n; i++) {
    if (a || b)
      k += i;
  }
  return k;
}

function andTestHelper(a, b, n)
{
  var k = 0;
  for (var i = 0; i < n; i++) {
    if (a && b)
      k += i;
  }
  return k;
}

if (!testName || testName == "truthies") {
  (function () {
     var opsies   = ["||", "&&"];
     var falsies  = [null, undefined, false, NaN, 0, ""];
     var truthies = [{}, true, 1, 42, 1/0, -1/0, "blah"];
     var boolies  = [falsies, truthies];

     // The for each here should abort tracing, so that this test framework
     // relies only on the interpreter while the orTestHelper and andTestHelper
     //  functions get trace-JITed.
     for each (var op in opsies) {
       for (var i in boolies) {
	 for (var j in boolies[i]) {
           var x = uneval(boolies[i][j]);
           for (var k in boolies) {
             for (var l in boolies[k]) {
               var y = uneval(boolies[k][l]);
               var prefix = (op == "||") ? "or" : "and";
               var f = new Function("return " + prefix + "TestHelper(" + x + "," + y + ",10)");
               f.name = prefix + "Test(" + x + "," + y + ")";
               f.expected = eval(x + op + y) ? 45 : 0;
               test(f);
             }
           }
	 }
       }
     }
   })();
}

function nonEmptyStack1Helper(o, farble) {
    var a = [];
    var j = 0;
    for (var i in o)
        a[j++] = i;
    return a.join("");
}

function nonEmptyStack1() {
    return nonEmptyStack1Helper({a:1,b:2,c:3,d:4,e:5,f:6,g:7,h:8}, "hi");
}

nonEmptyStack1.expected = "abcdefgh";
test(nonEmptyStack1);

function nonEmptyStack2()
{
  var a = 0;
  for (var c in {a:1, b:2, c:3}) {
    for (var i = 0; i < 10; i++)
      a += i;
  }
  return String(a);
}
nonEmptyStack2.expected = "135";
test(nonEmptyStack2);

function arityMismatchMissingArg(arg)
{
  for (var a = 0, i = 1; i < 10000; i *= 2) {
    a += i;
  }
  return a;
}
arityMismatchMissingArg.expected = 16383;
test(arityMismatchMissingArg);

function arityMismatchExtraArg()
{
  return arityMismatchMissingArg(1, 2);
}
arityMismatchExtraArg.expected = 16383;
test(arityMismatchExtraArg);

function MyConstructor(i)
{
  this.i = i;
}
MyConstructor.prototype.toString = function() {return this.i + ""};

function newTest()
{
  var a = [];
  for (var i = 0; i < 10; i++)
    a[i] = new MyConstructor(i);
  return a.join("");
}
newTest.expected = "0123456789";
test(newTest);

function shapelessArgCalleeLoop(f, a)
{
  for (var i = 0; i < 10; i++)
    f(i, a);
}

function shapelessVarCalleeLoop(f, a)
{
  var g = f;
  for (var i = 0; i < 10; i++)
    g(i, a);
}

function shapelessLetCalleeLoop(f, a)
{
  for (var i = 0; i < 10; i++) {
    let g = f;
    g(i, a);
  }
}

function shapelessUnknownCalleeLoop(f, g, a)
{
  for (var i = 0; i < 10; i++) {
    (f || g)(i, a);
    f = null;
  }
}

function shapelessCalleeTest()
{
  var a = [];
  shapelessArgCalleeLoop(function (i, a) a[i] = i, a);
  shapelessVarCalleeLoop(function (i, a) a[10 + i] = i, a);
  shapelessLetCalleeLoop(function (i, a) a[20 + i] = i, a);
  shapelessUnknownCalleeLoop(null, function (i, a) a[30 + i] = i, a);
  try {
    shapelessUnknownCalleeLoop(null, {hack: 42}, a);
  } catch (e) {
    if (e + "" != "TypeError: g is not a function")
      print("shapelessUnknownCalleeLoop: unexpected exception " + e);
  }
  return a.join("");
}
shapelessCalleeTest.expected = "0123456789012345678901234567890123456789";
test(shapelessCalleeTest);

function typeofTest()
{
  var values = ["hi", "hi", "hi", null, 5, 5.1, true, undefined, /foo/, typeofTest, [], {}], types = [];
  for (var i = 0; i < values.length; i++)
    types[i] = typeof values[i];
  return types.toString();
}
typeofTest.expected = "string,string,string,object,number,number,boolean,undefined,object,function,object,object";
test(typeofTest);

/* Keep these at the end so that we can see the summary after the trace-debug spew. */
print("\npassed:", passes.length && passes.join(","));
print("\nFAILED:", fails.length && fails.join(","));

