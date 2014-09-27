--TEST--
Test nullsafe operator : HHVM test "nullsafe-prop-not-allowed.php"
--FILE--
<?php
class C {
  public $x = 123;
}
function test($c) {
  return $c?->x;
}
--EXPECTF--
Fatal error: ?-> is not supported for property access in %s on line 6
