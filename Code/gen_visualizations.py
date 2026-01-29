import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import argparse
import os
import glob


# --- Configuration for Clinical Style ---
# Standard Goldmann sizes usually have specific conventions.
# We will use different markers/colors to distinguish them.
SIZE_STYLES = {
    'I': {'color': 'blue', 'marker': '.', 's': 30, 'label': 'Size I'},
    'II': {'color': 'green', 'marker': '.', 's': 50, 'label': 'Size II'},
    'III': {'color': 'red', 'marker': 'o', 's': 60, 'label': 'Size III (Standard)'},
    'IV': {'color': 'orange', 'marker': 'o', 's': 80, 'label': 'Size IV'},
    'V': {'color': 'black', 'marker': 'o', 's': 100, 'label': 'Size V'},
    'Unknown': {'color': 'gray', 'marker': 'x', 's': 30, 'label': 'Unknown'}
}


def plot_visual_field(file_path):
    print(f"Processing: {file_path}")

    # 1. Load Data
    try:
        df = pd.read_csv(file_path)
    except Exception as e:
        print(f"Error loading file: {e}")
        return

    # 2. Extract Metadata from Filename
    filename = os.path.basename(file_path)

    # Determine Eye Side
    eye_side = "Unknown Eye"
    if "Left" in filename:
        eye_side = "Left Eye (OS)"
        # For Left Eye: Temporal is Left (180), Nasal is Right (0)
        nasal_pos = 0
        temporal_pos = np.pi
    elif "Right" in filename:
        eye_side = "Right Eye (OD)"
        # For Right Eye: Temporal is Right (0), Nasal is Left (180)
        temporal_pos = 0
        nasal_pos = np.pi
    else:
        # Default fallback
        temporal_pos = 0
        nasal_pos = np.pi

    # Parse Date/Time for Title (Simple cleanup of the filename pattern you defined)
    # perimetry_YYYY-MM-DD_HH-MM-SS.csv
    display_date = filename.replace("perimetry_", "").replace(".csv", "").replace("Left_", "").replace("Right_", "")

    # 3. Setup Polar Plot (The "Goldmann" Chart)
    fig = plt.figure(figsize=(10, 8))
    ax = plt.subplot(111, projection='polar')

    # --- Clinical Formatting ---

    # Set Grid (Isopters) - Circles every 10 degrees up to 90
    ax.set_ylim(0, 90)
    ax.set_yticks(np.arange(10, 100, 10))
    ax.set_yticklabels([f"{x}°" for x in range(10, 100, 10)], fontsize=8, color='gray')

    # Set Meridians - Every 30 degrees
    ax.set_xticks(np.deg2rad(np.arange(0, 360, 30)))
    ax.tick_params(axis='x', pad=10)  # Move angle labels out a bit

    # Add Crosshairs (Horizontal and Vertical lines)
    ax.vlines(0, 0, 90, colors='black', linestyles='-', alpha=0.3)
    ax.vlines(np.pi / 2, 0, 90, colors='black', linestyles='-', alpha=0.3)
    ax.vlines(np.pi, 0, 90, colors='black', linestyles='-', alpha=0.3)
    ax.vlines(3 * np.pi / 2, 0, 90, colors='black', linestyles='-', alpha=0.3)

    # Label Nasal/Temporal
    if "Unknown" not in eye_side:
        ax.text(temporal_pos, 105, "TEMPORAL", ha='center', va='center', fontweight='bold', color='darkred')
        ax.text(nasal_pos, 105, "NASAL", ha='center', va='center', fontweight='bold', color='darkblue')

    # 4. Plot the Data Points
    # Assuming CSV Columns: Theta (Angle in Deg), Phi (Eccentricity/Radius in Deg), SizeIndex

    # Group by SizeIndex to apply different styles
    groups = df.groupby('SizeIndex')

    for name, group in groups:
        # Convert Theta (Degrees) to Radians for Matplotlib
        # Note: In most perimetry, 0 is East. Matplotlib is also 0=East.
        theta_rad = np.deg2rad(group['Theta'])

        # Phi is the radius (Eccentricity)
        r = group['Phi']

        # Get style
        style = SIZE_STYLES.get(str(name), SIZE_STYLES['Unknown'])

        ax.scatter(theta_rad, r,
                   c=style['color'],
                   marker=style['marker'],
                   s=style['s'],
                   alpha=0.75,
                   label=style['label'],
                   edgecolors='white',
                   linewidth=0.5)

    # 5. Final Touches
    plt.title(f"Visual Field Perimetry\n{eye_side} - {display_date}", fontsize=14, pad=20)

    # Legend (placed outside to not block vision)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)

    plt.tight_layout()

    # Save the plot
    output_filename = filename.replace(".csv", ".png")
    plt.savefig(output_filename, dpi=150)
    print(f"Saved visualization to: {output_filename}")

    # Show it
    plt.show()


def main():
    # Automatically find the most recent CSV if no file is specified
    list_of_files = glob.glob('*.csv')

    if not list_of_files:
        print("No CSV files found in the current directory.")
        return

    # Sort by creation time to get the newest one
    latest_file = max(list_of_files, key=os.path.getctime)

    print(f"Found {len(list_of_files)} CSV files.")
    print(f"Visualizing most recent: {latest_file}")
    print("-" * 30)

    plot_visual_field(latest_file)


if __name__ == "__main__":
    main()