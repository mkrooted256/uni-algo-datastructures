#=
# Варіант 6
# 1. Bubble sort
# 2. Quicksort
=#
using Random
using DataFrames
using CSV

function bubblesort!(v::Vector{T}) where T
    len = length(v)
#     println("init: ", v)

    cmps = 0
    swaps = 0

    for i in 1:len
        sorted::Bool = true
        for j in 1:len-1
            if v[j] > v[j + 1]
                v[j],v[j + 1] = v[j + 1],v[j] # Swap
                swaps+=1
                sorted = false
            end
            cmps+=1 # At least 1 comparison at the if
        end
        sorted && break
    end

#     println("sorted: ", v)

    return (cmps, swaps)
end

function quicksort!(v::Vector{T}, pretty=false, pivoter::Function=((v, lo, hi)->fld(lo+hi, 2))) where T
    N = 0
    function print_v_frame(v, p, lo, hi)
        len = length(v)
        println(N)
        N+=1
        println(v)
        print(" ")
        for i=1:len
            if     i==lo print("[  ")
            elseif i==p  print("^  ")
            elseif i==hi print("]  ")
            else print("   ") end
        end
        println()
    end

    cmps = 0
    swaps = 0

    # Pretty version
    # Divide and calculate new pivot point
    function divide_pretty!(v::Vector{T}, lo, hi, pivoter::Function)
        p = pivoter(v, lo, hi)
        pivot = v[p] # Select new pivot - val of middle element
        print_v_frame(v, p, lo, hi)
        while true
            # Find first inversion
            while v[lo] < pivot lo+=1; cmps+=1; end; cmps+=1;
            print_v_frame(v, p, lo, hi)
            while v[hi] > pivot hi-=1; cmps+=1; end; cmps+=1;
            print_v_frame(v, p, lo, hi)
            # If sorted: no inversions found and frame shrinked to 0 width, return pivot point
            cmps+=1
            lo ≥ hi && (println("lo>=hi"); return hi)
            # otherwise, swap inversion and continue
            v[lo], v[hi] = v[hi], v[lo]
            swaps+=1

            print_v_frame(v, p, lo, hi)
        end
        print_v_frame(v, p, lo, hi)
    end
    # Normal version
    function divide!(v::Vector{T}, lo, hi, pivoter::Function)
        p = pivoter(v, lo, hi)
        pivot = v[p] # Select new pivot - val of middle element
        while true
            # Find first inversion
            while v[lo] < pivot lo+=1; cmps+=1; end; cmps+=1;
            while v[hi] > pivot hi-=1; cmps+=1; end; cmps+=1;
            # If sorted: no inversions found and frame shrinked to 0 width, return pivot point
            cmps+=1
            lo ≥ hi && return hi
            # otherwise, swap inversion and continue
            v[lo], v[hi] = v[hi], v[lo]
            swaps+=1
        end
    end

    Task = Tuple{Int, Int}
    tasks::Vector{Task} = []

    # Initial task
    push!(tasks, (1, length(v)))
    divider = pretty ? divide_pretty! : divide!;

    while !isempty(tasks)
        lo, hi = pop!(tasks) # Get a task
        cmps+=1
        lo < hi || continue
        p = divider(v, lo, hi, pivoter) # Calculate pivot point index
        push!(tasks, (lo,p))    # Add new task for left
        push!(tasks, (p+1,hi))  #   and right partition
    end

    return (cmps, swaps)
end

function benchmark(algo::Function, vlen::Int, samples::Int, args...)
    # cmps, swaps
    stats = [0, 0]
    @time begin
        v = [1:vlen;]
        for i in 1:samples
            shuffle!(v)
            stats .+= algo(v, args...)
        end
    end
    stats = stats ./ samples
    println("avg cmps: ", stats[1], "     avg swaps: ", stats[2])
end

function generate_report()
    Samples = 5

    evenNs = ((n)->10*(2^n)).([0:12; 14])
    oddNs = ((n)->11*(2^n)).([0:12; 14])

    function report_algo(label, Ns, Samples, algo::Function)
        local df = DataFrame(Label = String[], N = Int[], Cmps = Int[], Swaps = Int[])
        for N in Ns
            v = [1:N;]
            stats = [0, 0]
            println(label, " ", N, "...")
            for i in 1:Samples
                shuffle!(v)
                stats .+= algo(v)
            end
            stats = fld.(stats,Samples)
            push!(df, ( label, N, stats[1], stats[2] ))
        end
        return df
    end

    df = report_algo("bubble-odd", oddNs, Samples, bubblesort!)
    append!(df, report_algo("bubble-even", evenNs, Samples, bubblesort!))
    append!(df, report_algo("quick-odd", oddNs, Samples, quicksort!))
    append!(df, report_algo("quick-even", evenNs, Samples, quicksort!))

    CSV.write("mk-sorting-report.csv", df)
end