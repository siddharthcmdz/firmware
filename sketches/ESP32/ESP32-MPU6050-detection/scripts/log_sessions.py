"""
log_session.py — capture an IMU session from an ESP32 via serial.

Usage:
    python log_session.py --port /dev/cu.usbserial-0001 --label walking_v1
    python log_session.py --port COM3 --label fall_drop_test_01 --duration 30
"""

import argparse
import pathlib
import datetime
import sys
import time
import serial

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--port', required=True, help='Serial port to read from (e.g. COM3 or /dev/cu.usbserial-0001)')
    p.add_argument('--baud', type=int, default=460800, help='Baud rate for serial communication (default: 460800)')
    p.add_argument('--label', required=True, help='Label for the session (e.g. walking_v1)')
    p.add_argument('--duration', type=float, default=60, help='Duration of the session in seconds (default: 60)')
    p.add_argument('--out-dir', default='C:\\RakshaBand\\data\\raw_sessions', help='Directory to save the session CSV file (default: C:\\RakshaBand\\data\\raw_sessions)')
    args = p.parse_args()

    out_dir = pathlib.Path(args.out_dir).expanduser()
    out_dir.mkdir(parents=True, exist_ok=True)

    timestamp = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    out_path = out_dir / f"{timestamp}_{args.label}.csv"

    print(f"Opening {args.port} @{args.baud} baud", file=sys.stderr)
    ser = serial.Serial(args.port, args.baud, timeout=1)
    time.sleep(0.5)  # Wait for serial connection to initialize
    ser.reset_input_buffer()

    print(f"Logging to {out_path}", file=sys.stderr)
    print(f"Press Ctrl+C to stop", file=sys.stderr)

    start = time.time()
    line_count = 0
    header_seen = False
    try:
        with open(out_path, "w", newline="") as f:
            # Write a session metadata header
            f.write(f"# session_label={args.label}\n")
            f.write(f"# session_start_iso={datetime.datetime.now().isoformat()}\n")
            f.write(f"# capture_host_unix={time.time()}\n")

            while True:
                if args.duration and (time.time() - start) > args.duration:
                    break

                raw = ser.readline()
                if not raw:
                    continue
                try:
                    line = raw.decode("utf-8").rstrip("\r\n")
                except UnicodeDecodeError:
                    print(f"Warning: could not decode line: {raw}", file=sys.stderr)
                    continue
                f.write(line + "\n")
                line_count += 1
                if line_count % 500 == 0:
                    elapsed = time.time() - start;
                    print(f"    {line_count} lines in {elapsed:.1f}s"
                          f"({line_count/elapsed:.1f} line/s)",
                          file=sys.stderr)
    except KeyboardInterrupt:
        print("\nStopped by user.", file=sys.stderr)
    finally:
        ser.close()
        elapsed = time.time() - start
        print(f"Wrote {line_count} lines in {elapsed:.1f}s -> {out_path}", file=sys.stderr)

if __name__ == "__main__":
    main()