# Differences Between CIM16 and CIM17 Standards

This document outlines the key differences between IEC 61970-301 CIM16 and CIM17 (Common Information Model) standards used in the electric power industry.

## Overview Comparison

| Aspect | CIM16 | CIM17 |
|--------|-------|-------|
| Release Year | 2013 | 2016 |
| IEC Standard | IEC 61970-301:2013 | IEC 61970-301:2016 |
| XML Namespace | http://iec.ch/TC57/2013/CIM-schema-cim16# | http://iec.ch/TC57/2016/CIM-schema-cim17# |
| Focus | Power system model exchange | Enhanced support for smart grids and distribution systems |

## Structural Changes

| Feature | CIM16 | CIM17 |
|---------|-------|-------|
| Inheritance Structure | Less granular class hierarchy | More refined class hierarchy with additional subclasses |
| Diagram Layout | Basic diagram layout capabilities | Enhanced diagram layout with more visualization options |
| Geographical Information | Limited geographical support | Improved geographical information modeling |
| Package Organization | 10 main packages | Expanded to 13 packages with better organization |

## Key Model Additions in CIM17

| Component | Description of Change |
|-----------|------------------------|
| Renewable Energy | Added classes for modeling renewable energy sources (solar, wind, etc.) |
| Energy Storage | Enhanced energy storage models with state-of-charge and cycling parameters |
| Distribution Systems | Expanded distribution network modeling capabilities |
| Power Electronics | New classes for inverters, converters, and other power electronic devices |
| Environmental Data | Added classes for environmental conditions affecting the power system |
| Microgrids | Support for microgrid modeling and operations |

## Class and Attribute Changes

| Class/Attribute | CIM16 | CIM17 |
|-----------------|-------|-------|
| `Equipment` | Basic equipment properties | Added condition monitoring attributes |
| `PowerTransformer` | Limited winding models | Enhanced transformer modeling with more detailed parameters |
| `ACLineSegment` | Basic line parameters | Added temperature dependency and advanced rating systems |
| `SynchronousMachine` | Basic machine model | Enhanced with dynamic response parameters |
| `Terminal` | Simple connection point | Added phases and measurement points |
| `EnergyConsumer` | Basic load modeling | Enhanced with time-varying load characteristics |
| `ProtectionEquipment` | Limited protection models | Expanded protection system modeling |
| `Switch` | Basic switching capabilities | Added detailed switch types and ratings |

## New Classes in CIM17

| New Class | Package | Description |
|-----------|---------|-------------|
| `SolarGeneratingUnit` | Generation | Models solar power generation units |
| `WindGeneratingUnit` | Generation | Enhanced wind power generation modeling |
| `BatteryUnit` | Generation | Models battery storage systems |
| `PowerElectronicsConnection` | Equipment | Models power electronics interfaces to the grid |
| `PowerElectronicsWindUnit` | Generation | Models wind farms with power electronics |
| `UsagePoint` | Metering | Enhanced metering point definition |
| `AssetHealthEvent` | Asset | Models equipment health events and conditions |
| `EnvironmentalAnalog` | Environmental | Models environmental measurements |

## Deprecated Elements in CIM17

| Deprecated Element | Replacement in CIM17 |
|--------------------|-----------------------|
| `StaticVarCompensator` | `StaticVarCompensatorDynamics` |
| `NonConformLoad` | Enhanced `EnergyConsumer` with new attributes |
| `ShuntCompensator.reactivePerSection` | `ShuntCompensator.reactance` and `ShuntCompensator.susceptance` |
| `RegulatingControl.discrete` | `RegulatingControl.discrete` with new enumeration values |
| `VoltageControlZone` | More detailed voltage control modeling |

## Data Type Changes

| Data Type | CIM16 | CIM17 |
|-----------|-------|-------|
| Date/Time Handling | Basic time-based parameters | More detailed time-series data support |
| Unit Systems | Limited unit definitions | Expanded unit systems and conversions |
| Measurement Types | Basic measurement types | Enhanced measurement classification |
| Power Flow Models | Standard power flow models | Added support for unbalanced power flow |

## Namespace Changes

| CIM16 Namespace | CIM17 Namespace |
|-----------------|-----------------|
| `http://iec.ch/TC57/2013/CIM-schema-cim16#` | `http://iec.ch/TC57/2016/CIM-schema-cim17#` |
| `http://iec.ch/TC57/CIM16#` | `http://iec.ch/TC57/CIM17#` |

## Profile Changes

| Profile | CIM16 | CIM17 |
|---------|-------|-------|
| Equipment (EQ) | Basic equipment models | Enhanced with condition monitoring |
| Topology (TP) | Basic network topology | Enhanced connectivity and switching models |
| State Variables (SV) | Limited state variables | Expanded state measurement support |
| Dynamics (DY) | Basic dynamics modeling | Enhanced dynamics with power electronics |
| Geographical (GL) | Limited geographical data | Enhanced geographical information |
| Diagram Layout (DL) | Basic diagram layout | Enhanced visualization capabilities |

## Implementation Considerations

When migrating from CIM16 to CIM17:

1. Update all namespace references
2. Review class inheritance hierarchies for structural changes
3. Add missing attributes to existing classes
4. Implement new classes for enhanced functionality
5. Update profile definitions and constraints
6. Adjust validation rules to accommodate new model elements
7. Update API interfaces for new model components
8. Modify database schemas to support new attributes and classes

## Compatibility Considerations

- CIM17 is generally backward compatible with CIM16 at the conceptual level
- Direct XML file compatibility requires transformation due to namespace and structural changes
- Tools and applications designed for CIM16 typically need updates to fully support CIM17
- Mixed CIM16/CIM17 environments may require interface adapters or transformation services
