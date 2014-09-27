--TEST--
Test nullsafe operator : HHVM test "nullsafe-call-7.php"
--FILE--
<?php
class C {
  public function foo() {
    echo "foo() was called\n";
    return $this;
  }
  public function bar() {
    echo "bar() was called\n";
    return $this;
  }
}
function f() {
  echo "f() was called\n";
}
function g() {
  echo "g() was called\n";
}
function main() {
  $obj = new C();
  $x = $obj?->foo(f())?->bar(g());
  var_dump($x);
  $obj = null;
  $x = $obj?->foo(f())?->bar(g());
  var_dump($x);
  echo "Done\n";
}
main();
--EXPECT--
f() was called
foo() was called
g() was called
bar() was called
object(C)#1 (0) {
}
f() was called
g() was called
NULL
Done
