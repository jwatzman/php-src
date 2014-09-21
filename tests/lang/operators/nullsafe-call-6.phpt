--TEST--
Test nullsafe operator : HHVM test "nullsafe-call-6.php"
--FILE--
<?php
class C {
  public function foo() {
    return new D();
  }
}
class D {}
function test($c) {
  return $c?->foo();
}
function main() {
  $c = new C();
  var_dump(test($c));
  echo "Done\n";
}
main();
--EXPECT--
object(D)#2 (0) {
}
Done
