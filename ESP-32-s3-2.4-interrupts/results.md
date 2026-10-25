| Метод        | К-сть хибних спрацювань | К-сть натисків | Затримка | Складність | With RC                                                                   |
|--------------|-------------------------|----------------|----------|------------|---------------------------------------------------------------------------|
| Без debounce | 5                       | 20             | 0        | Minimal    | With RC each press now counts as 5. Not sure I connected everything right |
| Time-based   | 1                       | 20             | 50       | Minimal    | Each press is now 2. Maybe the capacitor is discharging                   |
| State-based  | 0                       | 50             | 25       | Normal     | No changes in behaviour                                                   |
| Polling      | 0                       | 100            | ~40      | Normal+    | No changes in behaviour                                                   |