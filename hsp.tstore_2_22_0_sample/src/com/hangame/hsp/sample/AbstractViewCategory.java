
package com.hangame.hsp.sample;

import com.hangame.hsp.ui.HSPUiUri;

import java.util.Map;

public abstract class AbstractViewCategory {
    private final String categoryName;

    protected AbstractViewCategory(String categoryName) {
        this.categoryName = categoryName;
    }

    public String getCategoryName() {
        return categoryName;
    }

    public abstract Map<String, HSPUiUri> getViewMap();
}
