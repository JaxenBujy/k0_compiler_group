// Expected error: while condition must be Boolean
fun main() {
    var y: Int = 0;
    while (y + 1) {
        y = y + 1;
    }
}