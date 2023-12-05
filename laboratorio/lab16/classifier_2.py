import argparse
import random
import time

def main(args):
    time.sleep(args.start)
    with open(args.fa, "r"):
        pass
    
    random.seed(args.start)
    print("chr,start,end,classification")
    for line in open(args.csvin, "r"):
        if random.random() < 0.15:
            continue
        line = line.strip()
        c, s, e = line.split(",")
        print(f"{c},{s},{e},{'benign' if bool(random.getrandbits(1)) else 'pathogenic'}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Argument Parser Example")

    # Add arguments
    parser.add_argument("--fa", type=str, required=True, help="FASTA")
    parser.add_argument("--csvin", type=str, required=True, help="Input")
    parser.add_argument(
        "--start", type=int, required=False, default=0, help="An integer start value"
    )
    # Parse the command-line arguments
    args = parser.parse_args()
    main(args)
