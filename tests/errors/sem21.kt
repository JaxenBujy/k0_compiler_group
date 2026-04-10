// Expected error: nullable operands not allowed in arithmetic
fun main() {
    var a: Int? = 10;
    var b: Int = 5;
    var c: Int = a + b;
}