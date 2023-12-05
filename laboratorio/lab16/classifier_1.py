import argparse
import random
import time

def main(args):
    time.sleep(args.seed)
    with open(args.ref, "r"):
        pass
    
    random.seed(args.seed)
    f_out = open(args.output, "w")
    for line in open(args.input, "r"):
        if random.random() < 0.15:
            continue
        line = line.strip()
        c, p = line.split(":")
        s, e = p.split("-")
        print(f"{c}:{s}-{e},{'B' if bool(random.getrandbits(1)) else 'P'}", file=f_out)

    f_out.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Argument Parser Example")

    # Add arguments
    parser.add_argument("--ref", type=str, required=True, help="Reference")
    parser.add_argument("-i", "--input", type=str, required=True, help="Input")
    parser.add_argument(
        "--seed", type=int, required=False, default=0, help="An integer seed value"
    )
    parser.add_argument(
        "-o", "--output", type=str, required=True, help="Output file path"
    )

    # Parse the command-line arguments
    args = parser.parse_args()
    main(args)
