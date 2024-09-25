import argparse
import random
import sys
import time

def main(args):
    time.sleep(args.s)
    with open(args.reffa, "r"):
        pass

    with open(args.filein, "r") as fin:
        lines = fin.readlines()

    header = lines[0].strip()

    if args.out == "-":
        fout = sys.stdout
    else:
        fout = open(args.out, "w")

    random.seed(args.s)

    print(header, "prediction", sep=",", file=fout)
    for line in lines[1:]:
        if random.random() < 0.15:
            continue
        line = line.strip()
        r, cov = line.split(",")
        cov = int(cov)

        pred = "X"
        if args.ignore_cov:
            pred = "b" if bool(random.getrandbits(1)) else "p"

        print(r, cov, pred, sep=",", file=fout)

    if args.out != "-":
        fout.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Argument Parser Example")

    # Add arguments
    parser.add_argument("-r", "--reffa", type=str, required=True, help="FASTA")
    parser.add_argument("--filein", type=str, required=True, help="Input")
    parser.add_argument(
        "-s", type=int, required=False, default=0, help="An integer seed value"
    )
    parser.add_argument(
        "--out",
        type=str,
        help="Output file. Use - for stdout (default -).",
        default="-",
    )
    parser.add_argument("--ignore-cov", action="store_true", default=False, help="Ignore coverage value (default False).")
    # Parse the command-line arguments
    args = parser.parse_args()
    main(args)
