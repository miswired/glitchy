# Tasks: Documentation Rewrite

## Phase 0a: Verification of Live Documentation (NEW - Added 2026-02-01)

**Purpose**: Verify technical accuracy of documentation already published on master branch before creating new content.

- [ ] 0a.1 Create verification system infrastructure
  - [x] Create OpenSpec spec: `openspec/specs/documentation-verification/spec.md`
  - [x] Create `verifications/` directory in wiki
  - [x] Create `verifications/README.md` implementation guide
  - [x] Create `verifications/TEMPLATE.md`
  - [ ] Update agent instructions (AGENTS.md or CLAUDE.md)
- [x] 0a.2 Verify Tutorial-Getting-Started.md (COMPLETE - 2026-02-01)
  - [x] Extract and verify all technical claims (35/35 verified)
  - [x] Create `verifications/VERIFICATION-Tutorial-Getting-Started.md`
  - [x] Update verification index
- [x] 0a.3 Verify Tutorial-First-Glitch.md (COMPLETE - 2026-02-01)
  - [x] Extract and verify all technical claims (40/40 verified)
  - [x] Create `verifications/VERIFICATION-Tutorial-First-Glitch.md`
  - [x] Update verification index
- [ ] 0a.4 Verify HowTo-Setup-PlatformIO.md
  - [ ] Extract and verify all technical claims
  - [ ] Create `verifications/VERIFICATION-HowTo-Setup-PlatformIO.md`
  - [ ] Update verification index
- [ ] 0a.5 Verify HowTo-Flash-Firmware.md
  - [ ] Extract and verify all technical claims
  - [ ] Create `verifications/VERIFICATION-HowTo-Flash-Firmware.md`
  - [ ] Update verification index
- [ ] 0a.6 Verify HowTo-Configure-WiFi.md
  - [ ] Extract and verify all technical claims
  - [ ] Create `verifications/VERIFICATION-HowTo-Configure-WiFi.md`
  - [ ] Update verification index
- [ ] 0a.7 Verify HowTo-Update-WebGUI.md
  - [ ] Extract and verify all technical claims
  - [ ] Create `verifications/VERIFICATION-HowTo-Update-WebGUI.md`
  - [ ] Update verification index
- [ ] 0a.8 Verify HowTo-Setup-KiCad.md
  - [ ] Extract and verify all technical claims
  - [ ] Create `verifications/VERIFICATION-HowTo-Setup-KiCad.md`
  - [ ] Update verification index
- [ ] 0a.9 Verify HowTo-Troubleshoot.md
  - [ ] Extract and verify all technical claims
  - [ ] Create `verifications/VERIFICATION-HowTo-Troubleshoot.md`
  - [ ] Update verification index

## Phase 0b: Code Quality Improvements (Identified During Verification)

**Purpose**: Address minor code documentation and clarity issues identified during documentation verification.

- [ ] 0b.1 Add FAT32 requirement comment to SD card initialization
  - Location: Code/Glitchy/src/main.cpp near SD.begin() (line ~56)
  - Add comment: "// SD card must be formatted as FAT32"
  - Rationale: Makes filesystem requirement explicit for developers
  - Severity: Minor (improves code clarity)
  - Source: Tutorial-Getting-Started verification
- [ ] 0b.2 Explicitly set Access Point IP address
  - Location: Code/Glitchy/src/main.cpp in initWiFi() function
  - Add: WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  - Rationale: Makes IP configuration explicit instead of relying on ESP32 library default
  - Severity: Minor (improves code clarity)
  - Source: Tutorial-Getting-Started verification
- [ ] 0b.3 Fix Target_glitch file path reference case sensitivity
  - Location: Tutorial-First-Glitch.md, Part 3, step 3
  - Change: "Code/Target Examples/Target_Glitch/" to "Code/Target Examples/Target_glitch/"
  - Rationale: Actual directory uses lowercase 'g'; case-sensitive on Linux/Mac
  - Severity: Minor (prevents confusion on case-sensitive systems)
  - Source: Tutorial-First-Glitch verification

## Phase 0: Research & Source Verification (For New Documentation)

- [ ] 0.1 Compile authoritative source list for hardware security topics
  - Academic papers on fault injection and power analysis
  - Established security conferences (CHES, USENIX Security, IEEE S&P)
  - Books: "The Hardware Hacker" (Huang), "Power Analysis Attacks" (Mangard et al.)
  - ChipWhisperer documentation and research
- [ ] 0.2 Compile authoritative source list for electronics/embedded topics
  - ESP32-S3 Technical Reference Manual (Espressif official)
  - Arduino framework documentation
  - Component datasheets (FET, op-amps, ADC)
  - Application notes from manufacturers
- [ ] 0.3 Research and document glitching theory from academic sources
  - Voltage fault injection papers
  - Timing analysis research
  - Document source references for each claim
- [ ] 0.4 Research and document power analysis theory from academic sources
  - DPA/SPA foundational papers (Kocher et al.)
  - Statistical analysis methods
  - Document source references for each claim
- [ ] 0.5 Research circuit design principles from authoritative sources
  - Crowbar circuit design references
  - Analog front-end design application notes
  - Op-amp configuration references
- [ ] 0.6 Create source reference document (Reference-Sources.md)
  - Categorized list of all authoritative sources used
  - Brief description of each source's authority
  - Links to papers, datasheets, documentation

## Phase 1: Foundation & Structure

- [ ] 1.1 Create wiki directory structure (assets/diagrams/, tutorials/, how-to/, reference/, explanation/, classes/)
- [x] 1.2 Create new Home.md with Diataxis-organized navigation (LIVE on master)
- [ ] 1.3 Create documentation style guide (formatting conventions, heading levels, code blocks)
  - Accessibility guidelines (proper heading hierarchy, alt text requirements)
  - Consistent heading structure for screen reader navigation
  - Image/diagram alt text requirements
- [ ] 1.4 Archive Arduino-Setup.md with deprecation notice and migration path

## Phase 2: Tutorials (Learning-Oriented)

- [x] 2.1 Rewrite Tutorial-Getting-Started.md (first 30 minutes with Glitchy) (LIVE on master)
  - Prerequisites section
  - Estimated time
  - What you'll learn
  - Step-by-step with checkpoints
- [x] 2.2 Rewrite Tutorial-First-Glitch.md (migrate from current Guide) (LIVE on master)
  - Clear learning objectives
  - Expected outcomes at each step
  - Troubleshooting inline
- [ ] 2.3 Rewrite Tutorial-First-Analysis.md (migrate from current Guide)
  - Move IMPORTANT notes to top
  - Add safety section
  - Clear success criteria
- [ ] 2.4 Create Tutorial-Custom-Target.md (NEW)
  - Writing vulnerable code for practice
  - Setting up custom hardware targets

## Phase 3: How-To Guides (Task-Oriented)

- [x] 3.1 Create HowTo-Setup-PlatformIO.md (CRITICAL - currently missing) (LIVE on master)
  - Installation steps
  - Project configuration
  - Building and uploading
  - Common issues
- [x] 3.2 Rewrite HowTo-Flash-Firmware.md (LIVE on master)
  - Separate SD card vs firmware procedures
  - Version verification steps
  - Rollback procedure
- [x] 3.3 Rewrite HowTo-Configure-WiFi.md (LIVE on master)
  - When to use AP vs Client mode
  - Step-by-step for each mode
  - Connection verification
- [x] 3.4 Rewrite HowTo-Setup-KiCad.md (LIVE on master)
  - Opening Glitchy project files
  - Making modifications
  - Exporting for manufacturing
- [x] 3.5 Create HowTo-Troubleshoot.md (NEW) (LIVE on master)
  - Categorized by symptom
  - WiFi issues
  - Glitching failures
  - Power analysis problems
  - Web interface issues
- [x] 3.6 Create HowTo-Update-WebGUI.md (NEW) (LIVE on master)
  - Downloading releases
  - SD card preparation
  - Verification

## Phase 4: Reference Documentation (Lookup-Oriented)

- [ ] 4.1 Create Reference-Pinout.md (NEW)
  - Complete ESP32-S3 pin assignments
  - Pin function table
  - Electrical specifications
- [ ] 4.2 Create Reference-Web-API.md (NEW)
  - WebSocket protocol documentation
  - JSON message formats
  - Command reference
- [ ] 4.3 Create Reference-Parameters.md (NEW)
  - All glitch_param_t fields
  - Valid ranges and defaults
  - Effect of each parameter
- [ ] 4.4 Create Reference-BOM.md (NEW)
  - Complete Bill of Materials
  - Part numbers and suppliers
  - Substitution notes
- [ ] 4.5 Create Reference-Schematic.md (NEW)
  - Circuit block descriptions
  - Key component explanations
  - Links to datasheets
- [ ] 4.6 Create Reference-Glossary.md (NEW)
  - Technical terms: glitch, crowbar, side-channel, DPA, SPA, FET, etc.
  - Hardware terms: ADC, GPIO, SPI, UART
  - Security terms: bypass, fault injection, timing attack
- [ ] 4.7 Create Reference-FAQ.md (NEW)
  - Common questions organized by category
  - Quick answers with links to detailed docs
  - Troubleshooting quick reference
- [ ] 4.8 Create Reference-Changelog.md (NEW)
  - Version history (firmware + web GUI)
  - What changed in each release
  - Breaking changes highlighted
  - Migration notes between versions
- [ ] 4.9 Create Reference-Version-Compatibility.md (NEW)
  - Firmware ↔ Web GUI compatibility matrix
  - Hardware revision notes (for future revisions)
  - Deprecation notices

## Phase 5: Explanation Documentation (Understanding-Oriented)

- [ ] 5.1 Rewrite Explain-Glitching-Theory.md
  - Add conceptual diagrams
  - Expand physics explanation
  - Historical context
  - Link to academic papers
- [ ] 5.2 Create Explain-Power-Analysis.md (CRITICAL - currently TODO)
  - DPA/SPA theory
  - Statistical analysis basics
  - Why it works
- [ ] 5.3 Create Explain-Circuit-Design.md (CRITICAL - currently TODO)
  - FET crowbar circuit explanation
  - Analog front-end design
  - Design trade-offs
- [ ] 5.4 Create Explain-Architecture.md (NEW)
  - System block diagram explanation
  - Firmware module overview
  - Web interface architecture
  - Communication protocols
- [ ] 5.5 Create Explain-Security-Context.md (NEW)
  - Real-world applications
  - Defense strategies
  - Ethical considerations
  - Legal notes
- [ ] 5.6 Create Reference-Further-Learning.md (NEW)
  - Related projects (ChipWhisperer, etc.) with non-affiliation disclaimer
  - Academic resources and papers
  - Similar tools and platforms
  - Recommended learning paths
  - Books and courses
  - Clear disclaimer: "These are recommended resources; Glitchy is not affiliated with these projects"

## Phase 6: SVG Diagrams

- [ ] 6.1 Create diagram style guide
  - Color palette (dark mode compatible)
  - Font specifications
  - Line weights and spacing
  - Accessibility requirements
- [ ] 6.2 Create system-architecture.svg
  - ESP32-S3, FET, Target, Web Browser blocks
  - Connection flows
  - Signal labels
- [ ] 6.3 Create pinout-esp32s3.svg
  - Physical pin layout
  - Function color coding
  - Legend
- [ ] 6.4 Create glitch-timing-waveform.svg
  - Annotated voltage waveform
  - Timing labels
  - Normal vs glitched comparison
- [ ] 6.5 Create power-analysis-flow.svg
  - Data flow from target to analysis
  - ADC sampling visualization
  - Processing stages
- [ ] 6.6 Create crowbar-circuit.svg
  - Schematic-style diagram
  - Component labels
  - Signal flow
- [ ] 6.7 Create web-interface-map.svg
  - UI navigation diagram
  - Feature locations

## Phase 7: Classroom Materials

- [ ] 7.1 Create Class-Overview.md
  - Course structure
  - Prerequisites for instructors
  - Materials checklist
  - Timing recommendations
- [ ] 7.2 Create Lesson-01-Introduction.md
  - Hardware security landscape
  - Attack categories
  - Why glitching matters
  - Brief prerequisite overview with external links
  - (45 min)
- [ ] 7.3 Create Lesson-02-Hardware-Setup.md
  - Lab environment setup
  - Tool familiarization
  - Safety briefing
  - Embedded basics overview with external links
  - (45 min)
- [ ] 7.4 Create Lesson-03-Glitching-Lab.md
  - Hands-on glitching exercise
  - Parameter exploration
  - Success analysis
  - (45 min)
- [ ] 7.5 Create Lesson-04-Analysis-Lab.md
  - Power monitoring setup
  - Trace capture
  - Differential analysis
  - (45 min)
- [ ] 7.6 Create slides/ directory with presentation SVGs

## Phase 7a: Printable Quick Reference Materials

- [ ] 7.6a Create QuickRef-Pinout.pdf (NEW)
  - Single-page printable pinout diagram
  - Pin functions and color coding
  - Designed for lab bench posting
- [ ] 7.6b Create QuickRef-Parameters.pdf (NEW)
  - Glitch parameter quick reference
  - Valid ranges, defaults, effects
  - Single-page format
- [ ] 7.6c Create QuickRef-Commands.pdf (NEW)
  - Web interface quick reference
  - Common operations cheat sheet
  - WebSocket command summary
- [ ] 7.6d Create QuickRef-Troubleshooting.pdf (NEW)
  - Common problems and quick fixes
  - Decision tree format
  - Single-page printable

## Phase 7b: Community & Safety Documentation

- [ ] 7.7 Create Contributing.md (NEW)
  - How to contribute (code, docs, hardware)
  - Code style guidelines
  - Pull request process
  - Issue reporting guidelines (use GitHub Issues for doc feedback)
  - Citation format requirements (author, title, URL, date)
  - License compatibility checklist (CC-BY, MIT, Apache 2.0, public domain)
  - Attribution requirements for external content
  - Documentation update process and versioning guidelines
- [ ] 7.8 Create Safety-Guide.md (NEW)
  - Electrical safety guidelines
  - Soldering safety
  - ESD precautions
  - Legal and ethical use guidelines
  - Responsible disclosure practices

## Phase 7c: PDF Export

- [ ] 7.9 Set up PDF generation pipeline
  - Tool selection (Pandoc, markdown-pdf, or similar)
  - Template design matching wiki style
  - Dark/light mode variants
- [ ] 7.10 Generate complete documentation PDF
  - Combined single PDF with all sections
  - Table of contents with page numbers
  - Printable format optimization
- [ ] 7.11 Create PDF download instructions in wiki
  - Link to latest PDF release
  - Version information

## Phase 8: Quality Assurance

- [ ] 8.1 Review all documents for Diataxis compliance (no mixed types)
- [ ] 8.2 Test all internal links
- [ ] 8.3 Verify SVG rendering in GitHub wiki (light and dark mode)
- [ ] 8.3a Accessibility review
  - Verify proper heading hierarchy in all docs
  - Verify alt text on all images and diagrams
  - Test SVG accessibility (title/desc elements)
- [ ] 8.4 Walk through complete new user journey
- [ ] 8.5 Walk through contributor journey
- [ ] 8.6 Proofread for typos and consistency
- [ ] 8.7 Verify all external citations are complete and properly formatted
- [ ] 8.8 Audit all external content for license compatibility
- [ ] 8.9 Technical accuracy review against authoritative sources
  - Verify all glitching claims against research papers
  - Verify all power analysis claims against DPA literature
  - Verify all circuit descriptions against datasheets
  - Verify all ESP32-S3 claims against Espressif documentation
- [ ] 8.10 Expert review checkpoint (if available)
  - Have hardware security practitioner review technical content
  - Have embedded systems expert review firmware content
- [ ] 8.11 Update openspec/specs with final documentation capability
