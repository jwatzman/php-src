--TEST--
Test nullsafe operator : HHVM test "nullsafe-call-3.php"
--SKIPIF--
<?php
die("skip test imported from HHVM, which handles calls on null differently, included for completeness sake");
?>
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
function test1($c, $b) {
  return $c?->blah($b)->bar();
}
function test2($c, $b) {
  return $c?->blah($b)->bar()?->baz();
}
function main() {
  $c = new C();
  var_dump(test1($c, true));
  try {
    var_dump(test1(null, true));
  } catch (Exception $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
  }
  try {
    var_dump(test1($c, false));
  } catch (Exception $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
  }
  try {
    var_dump(test1(null, false));
  } catch (Exception $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
  }
  var_dump(test2($c, true));
  try {
    var_dump(test2(null, true));
  } catch (Exception $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
  }
  try {
    var_dump(test2($c, false));
  } catch (Exception $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
  }
  try {
    var_dump(test2(null, false));
  } catch (Exception $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
  }
}
main();
--EXPECT--
object(E)#3 (0) {
}
BadMethodCallException: Call to a member function bar() on a non-object (NULL)
BadMethodCallException: Call to a member function bar() on a non-object (NULL)
BadMethodCallException: Call to a member function bar() on a non-object (NULL)
object(F)#8 (0) {
}
BadMethodCallException: Call to a member function bar() on a non-object (NULL)
BadMethodCallException: Call to a member function bar() on a non-object (NULL)
BadMethodCallException: Call to a member function bar() on a non-object (NULL)
