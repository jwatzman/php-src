--TEST--
Test nullsafe operator : HHVM test "nullsafe-call-4.php"
--FILE--
<?php
class C {
  public function blah($x) {
    return $x ? new D() : null;
  }
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
function test($c, $b) {
  return $c->blah($b)?->bar()->baz();
}
function main() {
  $c = new C();
  var_dump(test($c, true));
  try {
    var_dump(test($c, false));
  } catch (Exception $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
  }
}
main();
--EXPECTF--
object(F)#2 (0) {
}

Catchable fatal error: Call to a member function baz() on null in %s on line %d
