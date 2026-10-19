# UAT Testing: Lessons Learned

How to verify Knob like a real user before calling a UI task done. The general lessons below were learned on the Velocity web app (Playwright); they apply to Knob the same way, with `screencapture` and real clicks on the menu bar instead of a browser.

## Knob-specific checks

### A. Start from the menu bar icon
Open Knob by clicking its menu bar icon, the way a user does. Default and landing-state bugs (wrong section open, stale volume, old preset name) only show up through the real entry point.

### B. The notch can hide the icon
On MacBooks with a notch, menu bar items can disappear behind it when the bar is crowded. Check the icon is visible with several other menu bar apps running.

### C. The popover must stay on screen
Check the popover's frame against the screen's visible frame, with every section expanded and with larger accessibility text sizes. Long sections must scroll inside the popover.

### D. Volume must match the system
After pressing the Mac volume keys, Knob's slider must match the macOS volume HUD, and moving Knob's slider must move the system volume. Test both directions.

### E. Act while audio state changes
Change things while the engine is starting, while the HDMI cable is unplugged and replugged, and across sleep/wake. A screenshot of a settled state proves little.

### F. Listen
If a change can affect audio (presets, EQ, effects, volume curve), ask the user to listen and confirm before calling it done. Meters moving is not the same as sounding right.

## General lessons (from Velocity)

## The core rule

DOM-count and click-success assertions are not UAT. They prove an element exists and a handler fired — they don't prove a real user could use the feature. Every lesson below is a case where `.count() > 0` or a `200` response passed while the feature was actually broken or inconsistent.

## Lessons

### 1. Look at the screenshot, don't just assert on it
A send button was clickable via `.click()` but sat visually underneath the floating PM Assistant bubble — completely unusable to a real user, invisible in any assertion that only checks `element.isVisible()` or bounding-box existence. Caught only by comparing bounding boxes for overlap, and by actually opening the screenshot.

**Do:** For any positioned/floating UI, check its bounding box against other known floating elements (nav bubbles, toasts) for overlap. Open and look at the screenshot before calling a screen done — every time, not spot-checks.

### 2. Verify the user actually sent by the screenshot they pasted
A user once pasted a screenshot to report a bug; it was from an unrelated application (wrong window). Proceeding to "fix" based on a misread screenshot wastes a round trip. If a screenshot doesn't match the feature under discussion, say so and ask for the right one — don't try to force-fit an unrelated image to the bug report.

### 3. Portal/dropdown positioning must be checked against where it's actually mounted, not just "does it render"
A mention-autocomplete dropdown and a date picker both defaulted to opening *below* their trigger. That's fine in a form near the top of a page — it's broken when the trigger sits at the bottom of the viewport (a chat compose box, a schedule row near the bottom of a panel), because the dropdown renders partially or fully off-screen. Both bugs passed `.count() > 0` (the element existed in the DOM) while being invisible to a real user.

**Do:** For any portal-positioned dropdown/calendar/picker, check `element.boundingBox()` and confirm it's fully within `{0,0,viewportWidth,viewportHeight}`. If a component is reused in a new layout context (bottom-anchored instead of top-anchored), re-check its positioning logic — don't assume a shared component's existing behavior generalizes to every place it gets dropped into.

### 4. Inline `style={{ top: undefined }}` does not clear a CSS class's `top` rule
When flipping a dropdown to open upward, omitting the `top` inline style (leaving it `undefined`) does **not** override a `top` value already declared on the element's CSS class — React simply omits the property from the style attribute, and the stylesheet rule wins. The fix must set an explicit override (`top: 'auto'`), not rely on omission. This bug looked identical to lesson #3 in symptoms (dropdown off-screen) but had a completely different root cause, and only showed up after the "fix" for #3 was already applied — always verify a fix with a fresh screenshot, don't assume the first plausible cause was the real one.

**Do:** When two inline styles compete with a CSS class for the same property, check computed style (`getComputedStyle`), not just the inline `style` object, to confirm which one actually won.

### 5. When two views get merged/unified (a toggle, a tab consolidation), diff their action sets against each other
Priority Inbox and My Threads were merged into one Inbox tab with a Mentions/Threads toggle. Mentions had a "Handled" (manual dismiss) action; Threads did not — an asymmetry invisible to anyone testing each view in isolation, but immediately obvious to a real user flipping the toggle and expecting the same actions to be available. The screenshots of each view individually looked fine; the bug only existed in the *comparison*.

**Do:** After merging or toggling between similar views, explicitly list each view's actions/fields side by side and diff them. Don't just confirm each renders — confirm they behave the same way for the same kind of item.

### 6. A quick client-side field reuse can silently break on the next background job
A thread's `has_reply` field looked like it could double as a manual "I've handled this" flag — until checking the scanner code showed it gets unconditionally recomputed from live Slack data on every periodic scan. Reusing it for a manual dismiss would have been silently undone by the next scan (15 minutes later), a bug that would never show up in an immediate Playwright check because the check runs before the next scan cycle.

**Do:** Before reusing an existing field for a new manual/user-controlled purpose, check whether anything else (a scanner, a cron job, a webhook) writes to that field asynchronously. If so, use a separate field, mirroring how the codebase's existing manual-override fields (e.g. a mention's `replied` flag) are already isolated from auto-computed ones.

### 7. Verify the actual data before declaring a rendering bug fixed (or existing)
A hypothesis that Slack mention tokens (`<@USERID>`) render as raw IDs in every view turned out to be only true for one specific new feature (a live, unprocessed Slack API feed) — a check against the real database showed the existing mention/thread scanner already resolves `<@USERID>` to a plain `@RealName` string at ingestion time, so the "same bug" did not exist in those older views. Assuming the bug was universal without checking the actual data would have led to a wasted, misdirected fix (or a false "still broken" report).

**Do:** Before fixing (or reporting) a data-rendering bug, query the real data (`curl` the API, check the DB) to confirm what shape the data actually has in each place it's displayed, rather than assuming all call sites share the same data pipeline.

### 8. A shared component's fix benefits (and must be checked against) every consumer
Fixing the date-picker off-screen bug inside the shared `CalendarPicker` component fixed it for all 7 places that use it, not just the one that surfaced the bug. When fixing a shared component, grep for every usage first — the fix is free leverage, but a wrong fix is also free breakage.

### 9. Status chips that appear and disappear can make the whole page jump
On the Knowledge Base page, header chips like "1 ready" and "1 admin-only" come and go as documents finish indexing or change visibility. With the stats in a wrapping row next to the title, each change toggled the header between one and two lines, so every list row moved about 30px while the user was aiming at it (a real click landed on the wrong element during UAT). A static screenshot looks fine; the bug only shows when you act during a state change.

**Do:** Give status counters a layout that cannot change height with their content (own row, fixed min-widths for badges like "Indexing"/"Ready"), and during UAT interact with the page *while* background state changes (polling, indexing), not only after it settles. Also check the last row's actions against the floating PM Assistant bubble at max scroll on mobile; pages need bottom padding to scroll clear of it.

## Process checklist

Before calling a UI task done:
- [ ] Screenshot every state, actually open and look at each one — no reasoning about screenshots you didn't view
- [ ] For any floating/positioned element, check bounding boxes against other floating UI and the viewport bounds
- [ ] Walk the real entry point (click the nav item) at least once, not only deep-linked URLs
- [ ] If two views were merged or made to share a pattern, diff their action sets/fields explicitly
- [ ] For any manual user action, check whether a background job could silently overwrite the same field
- [ ] Query real data before assuming a rendering bug's scope
- [ ] If a fix touches a shared component, grep for all its consumers
