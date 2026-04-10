// Expected error: nullable operand not allowed in unary not
fun main() {
    var b: Boolean? = false;
    var result = !b;
}