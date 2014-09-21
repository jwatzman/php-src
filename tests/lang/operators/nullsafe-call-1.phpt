--TEST--
Test nullsafe operator : HHVM test "nullsafe-call-1.php"
--FILE--
<?php
class C {
  public $prop;
  public function foo() {
    return new D();
  }
  public function blah($x) {
    return $x ? new D() : null;
  }
  public function yar($x) {}
}
class D {
  public function bar() {
    return new E();
  }
}
class E {
  public function baz() {
    return new F();
  }
}
class F {}
function test1($c) {
  return $c?->foo();
}
function test2($c) {
  return $c?->foo()?->bar()?->baz();
}
function test3($c, $b) {
  return $c->blah($b)?->bar();
}
function test4($c) {
  var_dump($c->prop?->bar()?->baz());
  if ($c->prop !== null) {
    var_dump($c->prop->bar()?->baz());
  } else {
    var_dump(null);
  }
}
function test5($c) {
  $x = null;
  $c?->yar($x = 123);
  return $x;
}
function main() {
  $c = new C();
  echo "test1:\n";
  var_dump(test1($c));
  var_dump(test1(null));
  echo "test2:\n";
  var_dump(test2($c));
  var_dump(test2(null));
  echo "test3:\n";
  var_dump(test3($c, true));
  var_dump(test3($c, false));
  echo "test4:\n";
  $c->prop = new D();
  var_dump(test4($c));
  $c->prop = null;
  var_dump(test4($c));
  echo "test5:\n";
  var_dump(test5($c));
  var_dump(test5(null));
  echo "Done\n";
}
main();
--EXPECT--
test1:
object(D)#2 (0) {
}
NULL
test2:
object(F)#2 (0) {
}
NULL
test3:
object(E)#3 (0) {
}
NULL
test4:
object(F)#4 (0) {
}
object(F)#2 (0) {
}
NULL
NULL
NULL
NULL
test5:
int(123)
int(123)
Done
