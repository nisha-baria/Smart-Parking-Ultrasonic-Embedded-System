import streamlit as st
import pandas as pd

st.set_page_config(page_title="Smart Parking Telemetry Dashboard", layout="wide", page_icon="🚗")

st.title("🚗 Smart Parking Real-Time Telemetry Dashboard")
st.markdown("Automated IoT Parking Management Prototype (ESP32 & Ultrasonic Sensors)")

# Sidebar Simulation Controls
st.sidebar.header("🕹️ Simulation Sensor Controls")
s1_dist = st.sidebar.slider("Slot 1 Distance (cm)", 5, 100, 15)
s2_dist = st.sidebar.slider("Slot 2 Distance (cm)", 5, 100, 80)
s3_dist = st.sidebar.slider("Slot 3 Distance (cm)", 5, 100, 20)
s4_dist = st.sidebar.slider("Slot 4 Distance (cm)", 5, 100, 90)

THRESHOLD = 35.0

slots_data = [
    {"Slot": "Slot 1", "Distance_cm": s1_dist, "Status": "OCCUPIED" if s1_dist < THRESHOLD else "FREE"},
    {"Slot": "Slot 2", "Distance_cm": s2_dist, "Status": "OCCUPIED" if s2_dist < THRESHOLD else "FREE"},
    {"Slot": "Slot 3", "Distance_cm": s3_dist, "Status": "OCCUPIED" if s3_dist < THRESHOLD else "FREE"},
    {"Slot": "Slot 4", "Distance_cm": s4_dist, "Status": "OCCUPIED" if s4_dist < THRESHOLD else "FREE"},
]

df = pd.DataFrame(slots_data)
free_count = (df['Status'] == 'FREE').sum()

# Top Metrics Row
col1, col2, col3 = st.columns(3)
col1.metric("Total Capacity", "4 Slots")
col2.metric("Available Slots", f"{free_count} / 4")
col3.metric("Gate Status", "OPEN (Access Granted)" if free_count > 0 else "LOCKED (Parking Full)")

if free_count == 0:
    st.error("🚨 ALERT: PARKING FULL! Entry Barrier Closed & Audio Alarm Active.")
else:
    st.success(f"🟢 SPACES AVAILABLE: {free_count} free parking bay(s).")

st.divider()

# Graphical Bay View
st.subheader("Live Parking Bay View")
cols = st.columns(4)

for idx, row in df.iterrows():
    with cols[idx]:
        is_occ = row["Status"] == "OCCUPIED"
        card_color = "#dc3545" if is_occ else "#198754"
        icon = "🚘" if is_occ else "🅿️"
        
        st.markdown(
            f"""
            <div style="background-color: {card_color}; padding: 16px; border-radius: 12px; text-align: center; color: white; box-shadow: 0 4px 6px rgba(0,0,0,0.3);">
                <h3 style="color: white; margin: 0; font-size: 1.3rem;">{row['Slot']}</h3>
                <div style="font-size: 55px; line-height: 1.2; margin: 8px 0;">{icon}</div>
                <h4 style="margin: 4px 0; letter-spacing: 1px;">{row['Status']}</h4>
                <p style="margin: 0; font-size: 0.9rem; opacity: 0.9;">Range: <b>{row['Distance_cm']} cm</b></p>
            </div>
            """, 
            unsafe_allow_html=True
        )

st.divider()
st.subheader("📊 Live Sensor Telemetry Log")
st.dataframe(df, use_container_width=True)