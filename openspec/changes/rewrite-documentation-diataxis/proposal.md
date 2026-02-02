# Change: Rewrite Documentation Using Diataxis Framework

## Why

The current wiki documentation has significant gaps that hinder both new users and contributors:
- 3 critical sections marked TODO (PlatformIO setup, Power Analysis Theory, Circuit Explanation)
- Deprecated Arduino IDE guide still prominently displayed
- No visual diagrams or architecture documentation
- Mixed documentation types causing confusion (tutorials vs how-to guides)
- No structured classroom/lesson materials for educational use
- Inconsistent formatting and incomplete troubleshooting

The Diataxis framework provides a proven structure that separates documentation by user need: learning (tutorials), doing (how-to), looking up (reference), and understanding (explanation).

## What Changes

### New Documentation Structure
- **Reorganize all content** into four Diataxis quadrants plus classroom section
- **Create missing documentation**: PlatformIO setup, power analysis theory, circuit explanation, architecture overview, troubleshooting guide
- **Add reference section**: Pinout, Web API protocol, parameters, BOM, schematic reference
- **Add classroom section**: Self-paced modules and instructor-led lesson plans

### Visual Improvements
- **Create SVG diagrams** for: system architecture, pinout, glitch timing, power analysis flow, waveform annotations
- **Design for dark mode**: Use `currentColor`, CSS variables, WCAG-compliant contrast
- **Embed directly in wiki**: Store SVGs in wiki repo for inline rendering

### Deprecation Handling
- **Archive Arduino-Setup.md** with clear migration notice
- **Remove TODO placeholders** by filling in actual content

### Quality Improvements
- **Consistent formatting** across all documents
- **Add estimated times** to tutorials and labs
- **Add prerequisites** to each document
- **Add troubleshooting sections** where appropriate

## Impact

- **Affected specs**: documentation (new capability)
- **Affected repositories**: glitchy.wiki (primary), glitchy/openspec (specs)
- **User impact**: Significantly improved onboarding and learning experience
- **Contributor impact**: Clear structure makes contributions easier
- **Breaking changes**: None - documentation only

## Success Criteria

1. All TODO items resolved with actual content
2. Every document categorized into exactly one Diataxis quadrant
3. Minimum 6 SVG diagrams created with dark mode support
4. Classroom section with at least 4 lesson plans
5. All documentation passes Diataxis validation (no mixed types)
6. New user can go from zero to first successful glitch using only documentation
