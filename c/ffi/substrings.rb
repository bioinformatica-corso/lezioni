#!/usr/bin/env ruby

require 'ffi'

module Substr
  extend FFI::Library
  ffi_lib FFI::Library::LIBC
  ffi_lib "./substrings.so"
  attach_function :substr2, [:string , :string ], :int
end

strings = ['abracadabra',
           'raccolta',
           'frazione']


puts strings.combination(2).map { |s1, s2| Substr.substr2(s1, s2) }.max

