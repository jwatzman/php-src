--TEST--
Test nullsafe operator : HHVM test "nullsafe-call-2.php"
--SKIPIF--
<?php
die("skip test imported from HHVM, relies on return types, included for completeness sake in case PHP RFC passes for them");
?>
--FILE--
<?php
class C {
  public function foo(): D {
    return new D();
  }
}
class D {}
function test($c): D {
  return $c?->foo();
}
function main() {
  $c = new C();
  var_dump(test($c));
  var_dump(test(null));
}
main();
--EXPECTF--
object(D)#2 (0) {
}

Fatal error: Value returned from test() must be of type D, null given in %s on line 9
