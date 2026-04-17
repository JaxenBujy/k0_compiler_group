// Expected error: nullable operands not allowed in logical expression
fun main() {
    var flag: Boolean? = true;
    var res: Boolean = flag && true;
}