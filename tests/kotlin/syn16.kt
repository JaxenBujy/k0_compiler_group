// Expected error: step in for-loop not supported in k0
fun main() {
    var i: Int;
    for (i in 1..10 step 2) {
        println(i);
    }
}