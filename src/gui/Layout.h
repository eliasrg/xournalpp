/*
 * Xournal++
 *
 * Handles the layout of the pages within a Xournal document
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include <gtk/gtk.h>

#include "gui/LayoutMapper.h"

#include "Rectangle.h"
#include "XournalType.h"

class XojPageView;
class XournalView;
class ScrollHandling;


/**
 * @brief The Layout manager for the XournalWidget
 *
 * This class manages the layout of the XojPageView's contained
 * in the XournalWidget
 */
class Layout {
public:
    Layout(XournalView* view, ScrollHandling* scrollHandling);
    virtual ~Layout();

public:
    /**
     * Increases the adjustments by the given amounts
     */
    void scrollRelative(double x, double y);

    /**
     * Changes the adjustments by absolute amounts (for pinch-to-zoom)
     */
    void scrollAbs(double x, double y);

    /**
     * Changes the adjustments in such a way as to make sure that
     * the given Rectangle is visible
     *
     * @remark If the given Rectangle won't fit into the scrolled window
     *         then only its top left corner will be visible
     */
    void ensureRectIsVisible(int x, int y, int width, int height);

    /**
     * Returns the height of the entire Layout
     */
    int getMinimalHeight() const;

    /**
     * Returns the width of the entire Layout
     */
    int getMinimalWidth() const;

    /**
     * Returns the Rectangle which is currently visible
     */
    Rectangle<double> getVisibleRect();


    /**
     * recalculate and resize Layout
     */
    void recalculate();

    /**
     * Performs a layout of the XojPageView's managed in this Layout
     * Sets out pages in a grid.
     * Document pages are assigned to grid positions by the mapper object and may be ordered in a myriad of ways.
     * only call this on size allocation
     */
    void layoutPages(int width, int height);

    /**
     * Updates the current XojPageView. The XojPageView is selected based on
     * the percentage of the visible area of the XojPageView relative
     * to its total area.
     */
    void updateVisibility();

    /**
     * Return the pageview containing co-ordinates.
     */
    XojPageView* getViewAt(int x, int y);

    /**
     * Return the page index found ( or std::nullopt if not found) at layout grid row,col
     *
     */
    std::optional<size_t> getIndexAtGridMap(size_t row, size_t col);

    /**
     * @brief Get the total padding above the page at the given index.
     *
     *  The size of this padding does not scale with pages as the user zooms in and out.
     * As such, it is often necessary to get _just_ this padding.
     *
     * @param pageIndex is the index of the XojPageView as returned by [getIndexAtGridMap]
     * @return the sum of the padding between pages above the page with the given index
     *         and any padding the user added vertically above all pages (i.e. in settings).
     */
    int getPaddingAbovePage(size_t pageIndex) const;

    /**
     * @brief Get the static padding added to the left of the current page.
     *
     *  This does not include padding added to center the page on the screen.
     *
     * @param pageIndex is the index of the XojPageView, as given by [getIndexAtGridMap]
     * @return the sum of the padding between pages left of the page at [pageIndex] and
     *         any horizontal padding the user decided to add (from settings)
     */
    int getPaddingLeftOfPage(size_t pageIndex) const;

protected:
    static void horizontalScrollChanged(GtkAdjustment* adjustment, Layout* layout);
    static void verticalScrollChanged(GtkAdjustment* adjustment, Layout* layout);

private:
    static void checkScroll(GtkAdjustment* adjustment, double& lastScroll);

    /**
     * Calls either the ScrollHandlingGtk or (when the Touch Workaround is enabled) the ScrollHandlingXournalpp
     * method to set the layout size by updating the horizontal and vertical GtkAdjustments
     */
    void setLayoutSize(int width, int height);

private:
    LayoutMapper mapper;

    XournalView* view = nullptr;
    ScrollHandling* scrollHandling = nullptr;

    std::vector<unsigned> widthCols;
    std::vector<unsigned> heightRows;

    double lastScrollHorizontal = -1;
    double lastScrollVertical = -1;

    guint hadjHandler = -1;
    guint vadjHandler = -1;

    /**
     * The last width and height of the widget
     */
    int lastWidgetWidth = 0;
    int lastWidgetHeight = 0;

    /**
     * The width and height of all our pages
     */
    size_t minWidth = 0;
    size_t minHeight = 0;

    /**
     * layoutPages invalidates the precalculation of recalculate
     * this bool prevents that layotPages can be called without a previously call to recalculate
     * Todo: we may want to remove the additional calculation in layoutPages, since we stored those values in
     */
    bool valid = false;
};
