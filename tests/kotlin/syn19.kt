// Expected error: safe call (?.) can only be used on nullable types
fun main() {
    val s: String = "hello";
    println(s?.length);
}