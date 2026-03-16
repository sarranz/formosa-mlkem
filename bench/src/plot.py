#!/usr/bin/env python3
import argparse

import matplotlib.pyplot as plt


def parse_bench_output(path):
    data = {}
    with open(path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    i = 0
    n = len(lines)
    while i < n:
        while i < n and lines[i].strip() == "":
            i += 1
        if i >= n:
            break

        op = lines[i].strip()
        i += 1

        counts = []
        while i < n and lines[i].strip() != "":
            counts.append(int(lines[i].strip()))
            i += 1

        data[op] = counts

        while i < n and lines[i].strip() == "":
            i += 1

    return data


def build_histograms(data_a, data_b, label_a, label_b, bins):
    ops = list(data_a.keys())
    for op in data_b.keys():
        if op not in data_a:
            ops.append(op)

    if not ops:
        raise ValueError("No operations found in either file.")

    fig, axes = plt.subplots(
        len(ops), 1, figsize=(10, 3.5 * len(ops)), squeeze=False
    )
    axes = axes[:, 0]

    for idx, op in enumerate(ops):
        ax = axes[idx]
        vals_a = data_a.get(op, [])
        vals_b = data_b.get(op, [])

        if vals_a:
            ax.hist(vals_a, bins=bins, alpha=0.5, label=label_a)
        if vals_b:
            ax.hist(vals_b, bins=bins, alpha=0.5, label=label_b)

        ax.set_title(op)
        ax.set_xlabel("Cycles")
        ax.set_ylabel("Frequency")
        ax.legend()
        ax.grid(True, alpha=0.25)

    return fig


def build_boxplots(data_a, data_b, label_a, label_b):
    ops = list(data_a.keys())
    for op in data_b.keys():
        if op not in data_a:
            ops.append(op)

    if not ops:
        raise ValueError("No operations found in either file.")

    fig, axes = plt.subplots(
        len(ops), 1, figsize=(10, 3.5 * len(ops)), squeeze=False
    )
    axes = axes[:, 0]

    for idx, op in enumerate(ops):
        ax = axes[idx]
        vals_a = data_a.get(op, [])
        vals_b = data_b.get(op, [])

        series = []
        labels = []
        if vals_a:
            series.append(vals_a)
            labels.append(label_a)
        if vals_b:
            series.append(vals_b)
            labels.append(label_b)

        if series:
            bp = ax.boxplot(
                series, tick_labels=labels, patch_artist=True, showfliers=True
            )
            colors = ["#4C72B0", "#DD8452"]
            for patch_i, patch in enumerate(bp["boxes"]):
                patch.set_facecolor(colors[patch_i % len(colors)])
                patch.set_alpha(0.6)

        ax.set_title(op)
        ax.set_ylabel("Cycles")
        ax.grid(True, axis="y", alpha=0.25)

    return fig


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file_a")
    parser.add_argument("file_b")
    parser.add_argument("--label-a", default="run A")
    parser.add_argument("--label-b", default="run B")
    parser.add_argument("--bins", type=int, default=40)
    parser.add_argument(
        "--output",
        default="bench_histograms.png",
        help="Output image path (e.g. bench_histograms.png)",
    )
    args = parser.parse_args()

    data_a = parse_bench_output(args.file_a)
    data_b = parse_bench_output(args.file_b)

    fig = build_histograms(data_a, data_b, args.label_a, args.label_b, args.bins)
    fig.tight_layout()
    fig.savefig(args.output, dpi=200, bbox_inches="tight")
    print(f"Saved histogram figure to: {args.output}")


if __name__ == "__main__":
    main()
