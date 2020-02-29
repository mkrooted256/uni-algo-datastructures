r_power(b, p) =
    p == 0 ? 1 : b*r_power(b, p - 1)


function r_euclid(a, b)
    r_euclid_w(a, b) =
        b == 0 ? a : r_euclid_w(b, a % b)
    # Ensure a is greater or equal than b
    if a < b c = a; a = b; b = c; end
    # Call worker
    r_euclid_w(a, b)
end

function r_fibbonaci(n)
    r_fibbonaci_w(n) =
        n < 2 ? 1 : r_fibbonaci_w(n - 1) + r_fibbonaci_w(n - 2)
    # Input validation
    n >= 0 || error("n must be non-negative")
    # Worker. Init condition: f_0 = f_1 = 1.
    r_fibbonaci_w(n)
end

mutable struct Peg
    bar::Vector{Int64}
    name::Char
end
Peg(name::Char) = Peg([], name)

function r_hanoi(n)
    # Pegs
    from = Peg([n:-1:1; ], 'A')
    mid = Peg('B')
    to = Peg('C')

    steps = 0

    function printpegs()
        function printpegrow(i, peg)
            if length(peg) >= i
                print('|' * '#'^peg[i] * ' '^(n - peg[i] + 1))
            else
                print('|' * ' '^(n + 1))
            end
        end

        for i in [n:-1:1; ]
            print(' ')
            printpegrow(i, from.bar)
            printpegrow(i, mid.bar)
            printpegrow(i, to.bar)
            println()
        end
        println('^'^(4(n+1)))
    end

    function r_hanoi_w(n, from, mid, to)
        if n > 0
            r_hanoi_w(n-1, from, to, mid)

            printpegs()
            steps += 1
            println("Step ", steps, ": Move disc ", n, " from ", from.name, " to ", to.name)

            push!(to.bar, pop!(from.bar))

            r_hanoi_w(n-1, mid, from, to)
        end
    end

    r_hanoi_w(n, from, mid, to)
    printpegs()
    println("Done in ", steps, " steps!")
end