// Expected error: for-loop range bounds must have the same type
fun main() {
    var j: Int;
    val start: Int = 1;
    val end: Long = 10;
    for (j in start..end) {
    }
}