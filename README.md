**Bus Topology:** I2C Multi-Master capable, configured in Single-Master mode at 100kbps.
- **Physical Layer:** GPIO Open-Drain stage with $V_{DD}$ pull-up integration for bus contention avoidance.
- **Frame Format:** 7-bit addressing (0x44) with MSB-aligned 8-bit R/W frame encapsulation.
- **Protocol Flow:** Single-shot measurement acquisition with 50ms clock stretching/wait state integration.
- **Data Acquisition:** 48-bit burst read sequence (16-bit Temp + 8-bit CRC + 16-bit RH + 8-bit CRC).
- **Signal Integrity:** Rise-time optimized timing register configuration for 8MHz HSI source.
- **Error Management:** HAL-level ACK/NACK validation and bus arbitration error trapping.
- **Peripheral Mapping:** AF1 multiplexed PB6/PB7 pins for dedicated I2C1 hardware peripheral.
- **Signal Processing:** Linear transfer function application for raw-to-physical unit conversion.
- **Compliance:** Full adherence to $I^2C$ Specification v6.0 for Standard-mode operations.
