using Plots, CSV, DataFrames

pyplot()

report = CSV.file("mk-sorting-report.csv")
df = DataFrame(report)

function plot_results()
    labels = ["bubble-even", "bubble-odd", "quick-even", "quick-odd"]
    d = []

    for l in labels
        push!(d, df[df.Label.==l, :])
    end

    p = plot(d[1].N, [d[1].Swaps d[1].Cmps d[3].Swaps d[3].Cmps], scale=:log2, legend=:topleft, fontfamily="Montserrat",
             title="Even Ns comparison", lab=["Bubble-Swaps" "Bubble-Cmps" "Quick-Swaps" "Quick-Cmps"])
    savefig(p, "../out/even.png")
    p = plot(d[1].N, [d[2].Swaps d[2].Cmps d[4].Swaps d[4].Cmps], scale=:log2, legend=:topleft, fontfamily="Montserrat",
             title="Odd Ns comparison", lab=["Bubble-Swaps" "Bubble-Cmps" "Quick-Swaps" "Quick-Cmps"])
    savefig(p, "../out/odd.png")
end