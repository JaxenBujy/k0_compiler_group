// Expected error: nullable operands not allowed in relational expression
fun main() {
    var p: Double? = 3.14;
    var q: Double = 2.71;
    if (p > q) {
        println("bigger");
    }
}