import numpy as np
import pandas as pd
import math

# --- 1. CONFIGURATION ---

# Define your 3 Age Groups (Years)
# Replace these with the actual mean ages of your study groups
AGE_GROUPS = {
    "Group_Young": 23,
    "Group_Middle": 31,
    "Group_Old": 51
}

# Define the 12 Meridians used in your study
MERIDIANS = [0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330]

# Stimuli to generate
# Keys match the internal model names, Values are the column headers for the CSV
STIMULI_MAP = {
    "V4e": "V4e",
    "I3e": "I3e",
    "I2e": "I2e"
}

# --- 2. MATHEMATICAL MODEL COEFFICIENTS (Grobbel et al. 2016) ---

# Base Intercept
INTERCEPT = 34.1081779

# Global Age Effects
COEF_AGE = -0.2572225
COEF_LN_AGE = 4.50957638

# Shape Coefficients (Base)
COEF_SHAPE = {
    "sin": -4.0582332,
    "cos": 7.65641149,
    "sin2": -3.1203881,
    "cos2": 5.25169817,
    "sincos2": 0.71985225
}

# Stimulus Specific Offsets (Main Effects)
# Note: V4e is mapped to III4e baseline to ensure realistic peripheral extent (approx 90 deg temporal)
# The paper's reference intercept (34) appears too small for V4e without the +31.46 offset.
STIM_OFFSETS = {
    "V4e": 31.4617962,  # Using III4e value as proxy for V4e
    "III4e": 31.4617962,
    "I3e": 0.85379893,
    "I2e": -22.390857,
    "I1e": -28.717235
}

# Interaction: Age * Stimulus
INTER_AGE_STIM = {
    "V4e": 0.17008673,  # Using III4e
    "III4e": 0.17008673,
    "I3e": -0.2098779,
    "I2e": -0.3074818
}

# Interaction: Ln(Age) * Stimulus
INTER_LNAGE_STIM = {
    "V4e": -3.2546707,  # Using III4e
    "III4e": -3.2546707,
    "I3e": 4.61051333,
    "I2e": 7.80908545
}

# Interaction: Shape * Stimulus (Dictionary of dictionaries)
INTER_SHAPE_STIM = {
    "V4e": {"sin": -3.693, "cos": 3.129, "sin2": -3.396, "cos2": 3.251, "sincos2": 0.224},
    "I3e": {"sin": -1.688, "cos": -1.880, "sin2": 0.012, "cos2": 0.772, "sincos2": -0.745},
    "I2e": {"sin": 0.913, "cos": -10.349, "sin2": 1.245, "cos2": -0.769, "sincos2": -0.658}
}

# Three-Way Interactions (Age * Shape * Stimulus)
# Only significant for Cosine term
INTER_AGE_SHAPE_STIM = {
    "V4e": -0.0769468,
    "I3e": -0.0771797,
    "I2e": -0.107209
}

INTER_LNAGE_SHAPE_STIM = {
    "V4e": 2.38335733,
    "I3e": 1.65190687,
    "I2e": 3.41882728
}


# --- 3. CALCULATION FUNCTION ---

def calculate_eccentricity(age, angle_deg, stimulus):
    """
    Calculates the predicted eccentricity (radius) in degrees.
    """
    # Convert angle to radians
    # Note: In Perimetry/Polar, 0 is usually Temporal (East), 90 Superior.
    # Grobbel Fig 3 says "180 Meridian ... Nasal", which implies standard polar (0=Temp OD).
    rad = np.radians(angle_deg)

    # Pre-calculate terms
    ln_age = np.log(age)
    sin_a = np.sin(rad)
    cos_a = np.cos(rad)
    sin_2a = np.sin(2 * rad)
    cos_2a = np.cos(2 * rad)
    sincos_2a = sin_a * cos_2a

    # 1. Base Model (Intercept + Age Effects)
    ecc = INTERCEPT + (COEF_AGE * age) + (COEF_LN_AGE * ln_age)

    # 2. Base Shape Effects
    ecc += (COEF_SHAPE["sin"] * sin_a) + \
           (COEF_SHAPE["cos"] * cos_a) + \
           (COEF_SHAPE["sin2"] * sin_2a) + \
           (COEF_SHAPE["cos2"] * cos_2a) + \
           (COEF_SHAPE["sincos2"] * sincos_2a)

    # 3. Stimulus Main Effect
    ecc += STIM_OFFSETS.get(stimulus, 0)

    # 4. Interaction: Age * Stimulus
    ecc += (INTER_AGE_STIM.get(stimulus, 0) * age) + \
           (INTER_LNAGE_STIM.get(stimulus, 0) * ln_age)

    # 5. Interaction: Shape * Stimulus
    s_int = INTER_SHAPE_STIM.get(stimulus, {})
    if s_int:
        ecc += (s_int.get("sin", 0) * sin_a) + \
               (s_int.get("cos", 0) * cos_a) + \
               (s_int.get("sin2", 0) * sin_2a) + \
               (s_int.get("cos2", 0) * cos_2a) + \
               (s_int.get("sincos2", 0) * sincos_2a)

    # 6. Three-Way Interaction: Age * Shape(Cos) * Stimulus
    ecc += (INTER_AGE_SHAPE_STIM.get(stimulus, 0) * age * cos_a) + \
           (INTER_LNAGE_SHAPE_STIM.get(stimulus, 0) * ln_age * cos_a)

    return round(ecc, 2)


# --- 4. GENERATION LOOP ---

print("Generating lookup tables...")

for group_name, age in AGE_GROUPS.items():
    data = []
    print(f"Processing {group_name} (Age: {age})...")

    for angle in MERIDIANS:
        row = {"Meridian_Deg": angle}

        for model_stim, csv_header in STIMULI_MAP.items():
            val = calculate_eccentricity(age, angle, model_stim)
            # Clamp values to realistic limits (e.g. max 90 deg)
            if val > 90: val = 90.0
            if val < 0: val = 0.0
            row[csv_header] = val

        data.append(row)

    # Create DataFrame and Save
    df = pd.DataFrame(data)
    filename = f"../Measurements/normal_values_{group_name.lower()}.csv"
    df.to_csv(filename, index=False)
    print(f"Saved {filename}")

print("\nDone! Please verify the ages in the script match your study groups exactly.")