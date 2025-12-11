package it.unibo.sdh;

import it.unibo.sdh.impl.view.DashboardView;

public class AppLauncher {
    public static void main(String[] args) {
        final var dashboardView = new DashboardView();
        dashboardView.display();
    }
}
