--TEST--
Test nullsafe operator : HHVM test "nullsafe-call-8.php"
--FILE--
<?php
class D {
  public function bar() {
    echo "D::bar() was called\n";
  }
}
class C {
  public function foo() {
    return new D();
  }
}
function test($obj) {
  $d = $obj?->foo();
  if ($d !== null) {
    $d->bar();
  }
}
function main() {
  echo "1:\n";
  test(null);
  echo "2:\n";
  test(new C());
  echo "Done\n";
}
main();
--EXPECT--
1:
2:
D::bar() was called
Done
