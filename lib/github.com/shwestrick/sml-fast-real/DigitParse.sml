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
      if Real.isFinite result andalso not (Real.== (result, Real.maxFinite)) then
        SOME result
      else
        NONE
    end
end

val floatTests = [
  "123.456",
  "-123.456",
  "1.23e4",
  "-1.23e-4",
  "0.0",
  "inf",
  "nan"
]

fun testFloats () =
    let
        val _ = print "\nTesting float parse\n"
        val _ = List.app (fn s => 
            let
                val _ = print ("Testing: " ^ s ^ "\n")
                val result = DigitParse.parseFloat s
                val _ = print ("  Parsed: " ^ (case result of 
                    SOME r => Real.toString r 
                  | NONE => "failed") ^ "\n")
            in
                ()
            end) floatTests
    in
        print "Done\n"
    end

val _ = testFloats ()