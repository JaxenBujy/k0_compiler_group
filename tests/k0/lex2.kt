fun main() {
    var a = 1;
    var b = 2;

    val req = a === b;
    val rneq = a !== b;

    val gte = a >= b;
    val lte = a <= b;
    val eq = a == b;
    val neq = a != b;

    val and = true && false;
    val or = true || false;

    val nn = a!!;
    val safe = a?.toString();
    val elvis = a ?: b;
    val nullable = a?;

    val rangeEx = 1 ..< 10;
    val rangeIn = 1 .. 10;

    val plus = a + b;
    val minus = a - b;
    val mul = a * b;
    val div = a / b;
    val mod = a % b;

    val assign = a = b;
    val gt = a > b;
    val lt = a < b;
    val not = !true;
    val dot = a.toString();

    a++;
    b--;

    a += b;
    a -= b;
}