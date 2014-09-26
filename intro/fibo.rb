#!/usr/bin/env ruby



ARGV.each do |limit|
  fib = Array.new()
  limit.to_i.times do |x|
    fib[x] = if x <= 1 then 1 else fib[x] = fib[x - 1] + fib[x - 2] end
  end
  puts fib
end
