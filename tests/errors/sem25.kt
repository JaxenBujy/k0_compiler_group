// Expected error: cannot increment nullable variable counter
fun main() {
    var counter: Int? = 0;
    counter++;
}