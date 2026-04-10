// Expected error: nullable operand not allowed in unary minus
fun main() {
    var n: Int? = -5;
    var m = -n;
}