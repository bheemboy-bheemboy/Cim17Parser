# Differences Between CGMES 2.4 and CGMES 3.0

This document outlines the key differences between Common Grid Model Exchange Standard (CGMES) versions 2.4 and 3.0 used for power system data exchange in the European electricity market.

## Version Overview

| Aspect | CGMES 2.4 | CGMES 3.0 |
|--------|-----------|-----------|
| Release Year | 2014 | 2020 |
| Based on CIM Version | IEC CIM16 (61970-301:2013) | IEC CIM17 (61970-301:2016) |
| ENTSO-E Specification | ENTSO-E CGMES 2.4 | ENTSO-E CGMES 3.0 |
| XML Schema Version | CGMES_2.4.15 | CGMES_3.0.0 |
| Primary Focus | Initial grid model exchange standard | Enhanced grid modeling for renewable integration and market coupling |

## Profile Changes

| Profile | CGMES 2.4 | CGMES 3.0 |
|---------|-----------|-----------|
| Equipment (EQ) | Basic equipment models | Enhanced with new classes for renewables and HVDC |
| Topology (TP) | Basic network topology | Same structure with updated relations |
| State Variables (SV) | Basic state variables | Enhanced with additional measurement types |
| Steady State Hypothesis (SSH) | Initial operating points | Expanded with control modes for renewables |
| Dynamics (DY) | Limited dynamics support | Significantly enhanced with new dynamic models |
| Geographical (GL) | Basic geographical data | Enhanced geographical information |
| Diagram Layout (DL) | Basic diagram layout | Same structure with updated relations |
| Boundary Set (EQBD) | Basic boundary definitions | Enhanced with HVDC boundaries |
| **New** Operation (OP) | Not present | New profile for operational data exchange |
| **New** Short Circuit (SC) | Not present | New profile for short circuit calculations |

## Namespace Changes

| CGMES 2.4 | CGMES 3.0 |
|-----------|-----------|
| `http://entsoe.eu/CIM/SchemaExtension/3/1#` | `http://entsoe.eu/CIM/SchemaExtension/3/4#` |
| `http://entsoe.eu/CIM/EquipmentCore/3/1#` | `http://entsoe.eu/CIM/EquipmentCore/3/4#` |
| `http://entsoe.eu/CIM/EquipmentOperation/3/1#` | `http://entsoe.eu/CIM/EquipmentOperation/3/4#` |
| `http://entsoe.eu/CIM/EquipmentShortCircuit/3/1#` | `http://entsoe.eu/CIM/EquipmentShortCircuit/3/4#` |
| Based on CIM16 namespaces | Based on CIM17 namespaces |

## Key Enhancements in CGMES 3.0

| Feature Area | Enhancements in CGMES 3.0 |
|--------------|----------------------------|
| Renewable Energy | New classes for modeling solar, wind, and other renewables |
| HVDC Systems | Enhanced HVDC modeling including multi-terminal HVDC |
| Power Electronics | New models for inverter-based resources |
| Dynamic Models | Expanded library of standard dynamic models |
| Short Circuit | Dedicated profile for short circuit analysis |
| Operational Data | New profile for operational data exchange |
| Market Integration | Enhanced support for market coupling data |
| Boundary Models | Improved boundary definitions for cross-border exchanges |
| Unbalanced Power Flow | Support for unbalanced conditions modeling |

## New Classes in CGMES 3.0

| Category | New Classes |
|----------|-------------|
| Renewable Generation | `SolarGeneratingUnit`, `WindGeneratingUnit`, `PhotoVoltaicUnit` |
| HVDC | `DCLine`, `DCLineSegment`, `DCNode`, `DCTerminal`, `DCConverterUnit` |
| Power Electronics | `PowerElectronicsConnection`, `PowerElectronicsUnit`, `PowerElectronicsConnector` |
| Market | `RegisteredResource`, `MarketDocument`, `TimeSeries` |
| Operations | `SwitchingOperation`, `OutageSchedule`, `OperationalLimitType` |
| Short Circuit | `FaultCausingElement`, `FaultPath`, `ShortCircuitAnalysis` |
| Dynamic Models | Various new standard dynamic model classes for different equipment types |

## Extended Attributes for Existing Classes

| Class | New Attributes in CGMES 3.0 |
|-------|----------------------------|
| `ACLineSegment` | Enhanced thermal rating properties, environmental parameters |
| `PowerTransformer` | Advanced transformer models, phase shifting parameters |
| `SynchronousMachine` | Enhanced dynamic response parameters, black start capability |
| `BusbarSection` | Enhanced electrical characteristics |
| `Control` | Additional control modes for renewable integration |
| `Terminal` | Enhanced phase connectivity |
| `Substation` | Additional geographical and operational attributes |
| `Switch` | Enhanced switching capabilities and ratings |

## Data Validation Rules

| Aspect | CGMES 2.4 | CGMES 3.0 |
|--------|-----------|-----------|
| Number of Rules | ~200 rules | ~350 rules |
| Rule Categories | Basic quality and consistency | Enhanced rules for renewables and dynamics |
| Implementation | XML schema validation | XML schema + additional business rules |
| Quality Checks | Basic model consistency | Enhanced quality metrics |

## Model Exchange Process Changes

| Process | CGMES 2.4 | CGMES 3.0 |
|---------|-----------|-----------|
| Instance Files | Separate XML files per profile | Same approach with additional profile files |
| Header Information | Basic header | Enhanced header with additional metadata |
| Boundary Exchange | Basic boundary set | Enhanced boundary definitions for HVDC and renewables |
| Model Merging | Basic merging rules | Enhanced rules for consistent merging |
| IGM/CGM Process | Initial IGM to CGM workflow | Enhanced workflow with quality gates |

## Implementation Impact

| Area | Impact of CGMES 3.0 |
|------|---------------------|
| Data Conversion | Requires mapping from CGMES 2.4 to 3.0 models |
| Tool Support | Tools need updates to support new profiles and classes |
| Data Volume | Significantly larger data sets due to more detailed models |
| Computational Requirements | Higher computational demands for handling more complex models |
| Validation | Enhanced validation rules require updated validation engines |
| Integration | New interfaces for market and operational data exchange |

## XML Schema Differences

| Feature | CGMES 2.4 | CGMES 3.0 |
|---------|-----------|-----------|
| Core Schema Files | 7 core schemas | 9 core schemas |
| Schema Structure | Profile-based structure | Same approach with additional profiles |
| Schema Complexity | Moderate complexity | Higher complexity with more nested elements |
| Reference Handling | Basic references | Enhanced referential integrity |

## Governance and Documentation

| Aspect | CGMES 2.4 | CGMES 3.0 |
|--------|-----------|-----------|
| Specification Documents | ENTSO-E CGMES 2.4 specification | ENTSO-E CGMES 3.0 specification with implementation guides |
| Governance | ENTSO-E working groups | ENTSO-E with enhanced stakeholder involvement |
| Conformity Assessment | Basic conformity testing | Enhanced conformity assessment framework |
| Implementation Timeline | Implemented 2014-2020 | Phased implementation starting 2020 |

## Migration Considerations

When migrating from CGMES 2.4 to CGMES 3.0:

1. Update all namespace references and schema versions
2. Implement support for new profiles (OP and SC)
3. Add missing attributes to existing classes
4. Implement new classes for enhanced functionality
5. Update validation rules and quality checks
6. Adapt model exchange processes for new metadata requirements
7. Update boundary models to support enhanced interconnection data
8. Modify dynamic model libraries to incorporate new standard models
9. Adapt tools and workflows for increased data volume and complexity
10. Enhance security measures for expanded data exchange requirements

## Practical Implementation Challenges

- Backward compatibility issues requiring parallel support for both versions
- Increased computational and storage requirements for larger, more detailed models
- Training requirements for staff to understand new modeling capabilities
- Coordination of migration timelines across multiple stakeholders
- Validation of converted models to ensure consistency and accuracy
- Integration with existing systems and workflows
- Management of transition period with mixed CGMES versions
