structure DigitParse =
(* TODO: Probably rename to SIMDParse *)
struct
  val fast_float_parse_chars = _import "fast_float_parse_chars" public:
        (char Array.array * int * int) -> real;

  fun parseFloat s =
    let
      val chars = Array.fromList (String.explode s)
      val result = fast_float_parse_chars (chars, 0, String.size s)
    in
      if Real.isFinite result andalso not (Real.== (result, Real.maxFinite)) then
        SOME result
      else
        NONE
    end

  fun parseChars (chars, start, len) =
    let
      val result = fast_float_parse_chars (chars, start, len)
    in
      if Real.isFinite result andalso not (Real.== (result, Real.maxFinite)) then
        SOME result
      else
        NONE
    end

  fun parseSlice slice =
    let
      val (arr, start, len) = ArraySlice.base slice
      val result = fast_float_parse_chars (arr, start, len)
    in
      if Real.== (result, Real.maxFinite) then
        NONE
      else
        SOME result
    end
end

val floatTests = [
  "123.456",
  "-123.456",
  "1.23e4",
  "-1.23e-4",
  "0.0",
  "inf",
  "+inf",
  "-inf",
  "infinity",
  "+infinity",
  "-infinity",
  "nan",
  "0.09073e-5",
  "0.09073e+5"
]

fun testFloats () =
    let
        val _ = print "\nTesting float parse\n"
        val _ = List.app (fn s => 
            let
                val _ = print ("Testing: " ^ s ^ "\n")
                val (r, _) = valOf (Real.scan (fn i => if i >= String.size s then NONE else SOME (String.sub (s, i), i + 1)) 0)
                val _ = print ("Real.scan parsed: " ^ Real.toString r ^ "\n")
                val chars = Array.fromList (String.explode s)
                val slice = ArraySlice.full chars
                val result = DigitParse.parseSlice slice
                val _ = print ("  Parsed: " ^ (case result of 
                    SOME res => Real.toString res 
                  | NONE => "failed") ^ "\n")
            in
                ()
            end) floatTests
    in
        print "Done\n"
    end

val _ = testFloats ()