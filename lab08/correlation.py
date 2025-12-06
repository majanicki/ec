import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

corr_TSPA_edges_avg = -0.719979
corr_TSPA_nodes_avg = -0.744409
corr_TSPA_edges_best_local = -0.3882
corr_TSPA_nodes_best_local = -0.554408
corr_TSPA_edges_ils = -0.574334
corr_TSPA_nodes_ils = -0.661709

corr_TSPB_edges_avg = -0.712166
corr_TSPB_nodes_avg = -0.650117
corr_TSPB_edges_best_local = -0.535785
corr_TSPB_nodes_best_local = -0.653134
corr_TSPB_edges_ils = -0.629357
corr_TSPB_nodes_ils = -0.730867

def make_plot(csv_name, title, x_label, y_label, out_png, corr_value):
    df = pd.read_csv(csv_name, header=None, names=["cost", "sim"])
    plt.figure()
    plt.scatter(df["cost"], df["sim"], s=6)

    plt.xlabel(x_label, fontsize=12, fontweight='bold')
    plt.ylabel(y_label, fontsize=12, fontweight='bold')
    plt.title(title.split('/')[1], fontsize=13, fontweight='bold')

    plt.text(
        0.98, 0.97,
        f"corr = {corr_value:.4f}",
        transform=plt.gca().transAxes,
        verticalalignment="top",
        horizontalalignment="right",
        fontsize=12,
        bbox=dict(facecolor="white", alpha=0.7, edgecolor="black", boxstyle="round,pad=0.3")
    )

    plt.grid(True, linestyle='--', alpha=0.3)
    m, b = np.polyfit(df["cost"], df["sim"], 1)
    plt.plot(df["cost"], m*df["cost"] + b, color='red', linewidth=1)

    plt.tight_layout()
    plt.savefig(out_png, dpi=300)
    plt.close()

instance_names = ["lab08/TSPA", "lab08/TSPB"]

for name in instance_names:
    if "TSPA" in name:
        c_edges_avg  = corr_TSPA_edges_avg
        c_nodes_avg  = corr_TSPA_nodes_avg
        c_edges_best = corr_TSPA_edges_best_local
        c_nodes_best = corr_TSPA_nodes_best_local
        c_edges_ils  = corr_TSPA_edges_ils
        c_nodes_ils  = corr_TSPA_nodes_ils
    else:
        c_edges_avg  = corr_TSPB_edges_avg
        c_nodes_avg  = corr_TSPB_nodes_avg
        c_edges_best = corr_TSPB_edges_best_local
        c_nodes_best = corr_TSPB_nodes_best_local
        c_edges_ils  = corr_TSPB_edges_ils
        c_nodes_ils  = corr_TSPB_nodes_ils

    make_plot(f"{name}_edges_avg.csv",
              f"{name}: Avg Similarity (Edges)",
              "Objective Value",
              "Avg Similarity",
              f"{name}_edges_avg.png",
              c_edges_avg)

    make_plot(f"{name}_nodes_avg.csv",
              f"{name}: Avg Similarity (Nodes)",
              "Objective Value",
              "Avg Similarity",
              f"{name}_nodes_avg.png",
              c_nodes_avg)

    make_plot(f"{name}_edges_best_local.csv",
              f"{name}: Similarity to Best Local (Edges)",
              "Objective Value",
              "Similarity",
              f"{name}_edges_best_local.png",
              c_edges_best)

    make_plot(f"{name}_nodes_best_local.csv",
              f"{name}: Similarity to Best Local (Nodes)",
              "Objective Value",
              "Similarity",
              f"{name}_nodes_best_local.png",
              c_nodes_best)

    make_plot(f"{name}_edges_ils.csv",
              f"{name}: Similarity to ILS (Edges)",
              "Objective Value",
              "Similarity",
              f"{name}_edges_ils.png",
              c_edges_ils)

    make_plot(f"{name}_nodes_ils.csv",
              f"{name}: Similarity to ILS (Nodes)",
              "Objective Value",
              "Similarity",
              f"{name}_nodes_ils.png",
              c_nodes_ils)
