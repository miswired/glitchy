<!-- OPENSPEC:START -->
# OpenSpec Instructions

These instructions are for AI assistants working in this project.

Always open `@/openspec/AGENTS.md` when the request:
- Mentions planning or proposals (words like proposal, spec, change, plan)
- Introduces new capabilities, breaking changes, architecture shifts, or big performance/security work
- Sounds ambiguous and you need the authoritative spec before coding

Use `@/openspec/AGENTS.md` to learn:
- How to create and apply change proposals
- Spec format and conventions
- Project structure and guidelines

Keep this managed block so 'openspec update' can refresh the instructions.

<!-- OPENSPEC:END -->

---

# Documentation Verification Process

When creating or modifying Glitchy documentation, all technical claims MUST be verified against authoritative sources.

## Quick Reference

- **Verification Spec**: See `openspec/specs/documentation-verification/spec.md` for detailed requirements
- **Implementation Guide**: See `../glitchy.wiki/verifications/README.md` for how-to instructions
- **Verification Files**: Located in `../glitchy.wiki/verifications/`
- **Template**: Use `../glitchy.wiki/verifications/TEMPLATE.md` to create new verification files

## When to Verify

Verification is required when:
- Creating new documentation files
- Modifying existing documentation with technical claims
- Publishing documentation from development branches to master

Verification is NOT required for:
- Typo fixes or formatting changes
- Non-technical content (contributing guidelines, general explanations without specific claims)
- Changes to verification files themselves

## Verification Workflow

### 1. Before Creating Documentation

When creating new documentation:
1. Research authoritative sources FIRST (datasheets, TRMs, academic papers, source code)
2. Write documentation based on verified information
3. Create verification file documenting sources used
4. Mark hardware-dependent claims for testing

### 2. Before Modifying Documentation

When updating existing documentation:
1. Read existing verification file if it exists
2. Verify any new claims being added
3. Update verification file with new sources
4. Mark changed claims that need re-verification

### 3. Publishing Documentation

Before publishing to master branch:
1. Ensure verification file exists and is complete
2. All claims marked with confidence levels (HIGH/MEDIUM/LOW/UNKNOWN)
3. Hardware-dependent claims marked "⚠️ Needs Testing" if not tested
4. Verification index updated in `verifications/README.md`
5. OpenSpec task marked complete

## Creating Verification Files

### File Naming
`verifications/VERIFICATION-{exact-document-filename}.md`

**Examples**:
- `Tutorial-Getting-Started.md` → `VERIFICATION-Tutorial-Getting-Started.md`
- `HowTo-Setup-PlatformIO.md` → `VERIFICATION-HowTo-Setup-PlatformIO.md`

### File Structure

Copy `verifications/TEMPLATE.md` and fill in:

```markdown
# Verification: Document-Name.md
**Status**: In Progress
**Verified**: X/Y claims
**Last Updated**: YYYY-MM-DD

## Category Name Claims

### Claim: Exact claim text from document
- **Source**: Specific reference (file:line, doc page, URL)
- **Confidence**: HIGH|MEDIUM|LOW|UNKNOWN
- **Status**: ✓ Verified | ❌ Incorrect | ⚠️ Needs Testing | ? Unknown
- **Notes**: Context, corrections, or test requirements
```

## Confidence Levels

- **HIGH**: Official docs (TRM, datasheets), verified in code, tested on hardware
- **MEDIUM**: Inferred from verified sources, secondary sources (papers, books)
- **LOW**: Assumptions, anecdotal evidence, incomplete sources
- **UNKNOWN**: No source found, needs investigation

## Authoritative Sources

### PRIMARY (Preferred)
- **Official Hardware Docs**: Espressif ESP32-S3 TRM/Datasheet, component datasheets
- **Project Code**: Source files with line numbers (`pins.h:15`)
- **Schematics**: KiCad files with references
- **Hardware Testing**: Documented test results on physical Glitchy board

### SECONDARY (When Primary Unavailable)
- **Academic Papers**: Peer-reviewed (CHES, IEEE S&P, USENIX Security)
- **Technical Books**: "Power Analysis Attacks" (Mangard et al.), etc.
- **Framework Docs**: PlatformIO, Arduino official documentation

### INVALID (Do Not Use)
- Blog posts, forums, Reddit
- AI-generated content without verification
- Assumptions or guesses
- Undocumented claims

## Status Icons

- **✓ Verified**: Claim confirmed against authoritative source
- **❌ Incorrect**: Claim contradicts source, needs correction
- **⚠️ Needs Testing**: Requires hardware testing or validation
- **? Unknown**: No source found, needs investigation

## Hardware Testing

Some claims require testing on physical Glitchy hardware:
- WiFi SSID, IP addresses, network behavior
- Web interface behavior
- Timing measurements
- Power consumption values
- Pin voltages

**Process**:
1. Mark as "⚠️ Needs Testing" with confidence LOW/UNKNOWN
2. Document what needs testing in verification file
3. After testing: Update status to "✓ Verified", confidence to HIGH, source to "TESTED"
4. Record test procedure and results

## Updating Verification Index

After creating or updating verification file:

1. Open `../glitchy.wiki/verifications/README.md`
2. Update status table with:
   - Document status (In Progress / Complete / Needs Review)
   - Verified claim count
   - Issues found count
   - Last updated date
3. Update overall progress count

## OpenSpec Integration

- **High-level tracking**: Mark tasks in `openspec/changes/rewrite-documentation-diataxis/tasks.md`
- **Detailed tracking**: Record verification details in verification files
- **Relationship**: OpenSpec = WHAT to verify, Verification files = HOW claims were verified

## Example Workflow

**Scenario**: Updating Tutorial-Getting-Started.md

1. Read existing `VERIFICATION-Tutorial-Getting-Started.md` (if exists)
2. Make changes to tutorial
3. Identify new/changed technical claims
4. Research sources for new claims:
   - Check `pins.h` for pin definitions
   - Check ESP32-S3 TRM for hardware specs
   - Check main.cpp for WiFi behavior
5. Update verification file:
   - Add new claims with sources
   - Update confidence levels
   - Mark hardware tests needed
6. Update verification index README
7. Mark OpenSpec task complete

## Common Patterns

### Verifying Pin Assignments
```markdown
### Claim: Pin 15 controls the power glitch FET
- **Source**: Code/Glitchy/include/pins.h:15 `#define POWER_GLITCH_PIN 15`
- **Confidence**: HIGH (verified in source code)
- **Status**: ✓ Verified
- **Notes**: Cross-referenced with KiCad schematic Rev 1.0
```

### Verifying Hardware Specs
```markdown
### Claim: ESP32-S3 has 45 GPIO pins
- **Source**: ESP32-S3 Technical Reference Manual v1.1, Section 2.2, p.47
- **Confidence**: HIGH (official Espressif documentation)
- **Status**: ✓ Verified
```

### Hardware Test Needed
```markdown
### Claim: Default WiFi SSID is "Glitchy"
- **Source**: Not found in code or configuration
- **Confidence**: LOW (assumed from project name)
- **Status**: ⚠️ Needs Testing
- **Notes**: Check main.cpp WiFi.softAP() call. Test on hardware to confirm.
```

### Theory/Explanation
```markdown
### Claim: Voltage glitching works by momentarily dropping VCC
- **Source**: "Power Analysis Attacks" (Mangard et al.), Ch.8; "Fault Attacks on Secure Embedded Devices" (Bar-El, 2004)
- **Confidence**: HIGH (established academic research)
- **Status**: ✓ Verified
```

## Questions?

- **Process questions**: See `../glitchy.wiki/verifications/README.md`
- **Requirements**: See `openspec/specs/documentation-verification/spec.md`
- **Issues**: Open GitHub issue for documentation feedback