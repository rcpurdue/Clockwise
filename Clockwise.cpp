// Clockwise.cpp, rcampbel@purdue.edu, 2023-03-08

#include "Clockwise.h"
#include "FreeSansBold12pt7b.h"

// Small rects that "fill" ea quadrant
#define FILL_W      20
#define FILL_H      33

#define FONT_OFFSET 24
#define TEST_MODE   true

void Clockwise::drawWatchFace() {
    int days_per_mo[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    display.fillScreen(GxEPD_WHITE);  // TODO Remove after change to differential display logic
    display.setFont(&FreeSansBold12pt7b);

    // Q1: year number, month number
    drawQuadrant(0, 0, String(tmYearToCalendar(currentTime.Year)), currentTime.Month, 12, false);

    // Q2: month name, day number
    if (currentTime.Month == 4 || currentTime.Month == 5 || currentTime.Month == 7) {
        drawQuadrant(100, 0, monthStr(currentTime.Month), currentTime.Day, days_per_mo[currentTime.Month-1], false);
    } else {
        drawQuadrant(100, 0, monthShortStr(currentTime.Month), currentTime.Day, days_per_mo[currentTime.Month-1]
                     , false);
    }

    // Q3: 24-hour number, minute number
    drawQuadrant(100, 100, String(currentTime.Hour), currentTime.Minute, 60, true);  // true: show leading zero

    // Q4: AM/PM, 0-12 hour number
    drawQuadrant(0, 100, currentTime.Hour < 12 ? "AM" : "PM", ((currentTime.Hour+11)%12)+1, 12, false);
}

void Clockwise::drawQuadrant(int up_left_x, int up_left_y, const String &center_text, int num_fill, int total,
                             bool lead_zero) {

    int16_t text_x, text_y;  // Coordinates for drawing text
    uint16_t text_w, text_h;  // To get text bounds
    int of_twelve; // Number of "fill" rects to fill

    // Draw fill rects and fill text

    // Coordinates for fill rects, in order from start to finish, clockwise TODO Move this!
    int16_t fill_x[] = {FILL_W*2, FILL_W*3, FILL_W*4, FILL_W*4, FILL_W*4, FILL_W*3,
                        FILL_W*2, FILL_W*1, FILL_W*0, FILL_W*0, FILL_W*0, FILL_W*1};
    int16_t fill_y[] = {FILL_H*0, FILL_H*0, FILL_H*0, FILL_H*1, FILL_H*2, FILL_H*2,
                        FILL_H*2, FILL_H*2, FILL_H*2, FILL_H*1, FILL_H*0, FILL_H*0};

    // Calc how many of the 12 fill rects should be filled
    of_twelve = (total == 12) ? num_fill : int( (float)num_fill / (float)total * 12.0) + 1;

    // Track coordinates of the leading ("final") rect
    int final_rect_x;
    int final_rect_y;

    // Draw fill rects
    for (int i=0; i<of_twelve; i++) {
        final_rect_x = up_left_x+fill_x[i];
        final_rect_y = up_left_y+fill_y[i];
        display.fillRect(final_rect_x, final_rect_y, FILL_W, FILL_H, GxEPD_BLACK);
    }

    // Draw fill text

    // Set coordinates for drawing fill text
    text_x = final_rect_x;
    text_y = final_rect_y + FONT_OFFSET;

    if (num_fill >= 10 || lead_zero) {
        // Cases where value to display has two digits but there's not enough space to draw both:
        switch (of_twelve) {
            case 1:
                // Print at pos 1, L-to-R, but add a fill in pos 2 for overflow
                display.fillRect(up_left_x+fill_x[1], up_left_y+fill_y[1], FILL_W, FILL_H, GxEPD_BLACK);
                break;
            case 3:
                // Print at prev pos, L-to-R, which is pos -1 (use -2 because coord array is 0-based)
                text_x = up_left_x+fill_x[of_twelve-2];
                text_y = up_left_y+fill_y[of_twelve-2] + FONT_OFFSET;
                break;
            case 4:
            case 5:
            case 10:
            case 11:
                // Split up digits into "lead" & "follow"

                // Draw "follower" digit
                display.setTextColor(GxEPD_WHITE);
                display.setCursor(up_left_x+fill_x[of_twelve-2], up_left_y+fill_y[of_twelve-2] + FONT_OFFSET);
                display.print(num_fill % 10);

                // Change regular value to display to be a single "lead" digit
                num_fill /= 10;
                lead_zero = false;
                break;
        }
    }

    // Draw value to display inside fill

    String disp_num_fill(num_fill);

    if (lead_zero && num_fill < 10) {
        disp_num_fill = "0" + disp_num_fill;
    }

    display.setTextColor(GxEPD_WHITE);
    display.setCursor(text_x, text_y);
    display.print(disp_num_fill );

    // Draw center rect w/text
    display.drawRect(up_left_x+FILL_W, up_left_y+FILL_H, FILL_W * 3, FILL_H, GxEPD_BLACK);
    display.getTextBounds(center_text, 0, 0, &text_x, &text_y, &text_w, &text_h);
    display.setCursor(up_left_x+50-0.5*text_w, up_left_y+FILL_H+FONT_OFFSET);
    display.setTextColor(GxEPD_BLACK);
    display.print(center_text);

    // Draw quadrant outline
    display.drawRect(up_left_x, up_left_y, 100, 100, GxEPD_BLACK);
    display.drawRect(up_left_x+1, up_left_y+1, 98, 98, GxEPD_WHITE);
}
