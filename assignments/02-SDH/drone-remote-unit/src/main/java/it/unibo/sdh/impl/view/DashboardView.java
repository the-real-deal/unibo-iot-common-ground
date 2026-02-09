package it.unibo.sdh.impl.view;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Objects;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.impl.controller.DashboardController;

public class DashboardView extends JFrame {

	private static Logger logger = LoggerFactory.getLogger(DashboardView.class);

	private JButton takeOffButton;
	private JButton landButton;
	private JTextField distanceDisplay;
	private JTextField hangarStateDisplay;
	private JTextField droneStateDisplay;
	private JLabel hangarInAlarmNotification;

	private DashboardController controller;

	public DashboardView() {
		super(".:: Drone Remote Unit — DASHBOARD ::.");

		setSize(640, 480);
		this.setResizable(false);

		final JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));
		mainPanel.setBorder(BorderFactory.createEmptyBorder(0, 20, 0, 20));

		final JPanel stateViewPanel = new JPanel();
		stateViewPanel.setLayout(new GridLayout(1, 3));

		final JPanel hangarStateView = new JPanel();
		hangarStateView.setLayout(new BoxLayout(hangarStateView, BoxLayout.Y_AXIS));
		hangarStateView.add(new JLabel("HANGAR state"));

		final JPanel hangarStateViewRow = new JPanel();
		hangarStateViewRow.setLayout(new BoxLayout(hangarStateViewRow, BoxLayout.X_AXIS));
		hangarStateDisplay = new JTextField();
		hangarStateDisplay.setEditable(false);
		hangarStateDisplay.setMaximumSize(new Dimension(150, 25));
		hangarStateViewRow.add(hangarStateDisplay);
		hangarStateView.add(hangarStateViewRow);

		final JPanel droneStateView = new JPanel();
		droneStateView.setLayout(new BoxLayout(droneStateView, BoxLayout.Y_AXIS));
		droneStateView.add(new JLabel("DRONE state"));
		droneStateDisplay = new JTextField();
		droneStateDisplay.setEditable(false);
		droneStateDisplay.setMaximumSize(new Dimension(150, 25));
		droneStateView.add(droneStateDisplay);

		this.hangarInAlarmNotification = new JLabel("Attention! Hangar in ALARM state!");
		this.hangarInAlarmNotification.setVisible(false);
		droneStateView.add(this.hangarInAlarmNotification);

		stateViewPanel.add(hangarStateView);
		stateViewPanel.add(Box.createRigidArea(new Dimension(0, 0)));
		stateViewPanel.add(droneStateView);
		mainPanel.add(stateViewPanel);
		mainPanel.add(new JSeparator());

		final JPanel infoLine = new JPanel();
		infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));
		distanceDisplay = new JTextField("-- mt.");
		distanceDisplay.setEditable(false);
		distanceDisplay.setMaximumSize(new Dimension(150, 25));
		infoLine.add(new JLabel("Distance to ground: "));
		infoLine.add(distanceDisplay);

		mainPanel.add(infoLine);
		mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));
		mainPanel.setMinimumSize(new Dimension(this.getWidth(), this.getHeight()));

		takeOffButton = new JButton("Take Off");
		takeOffButton.setEnabled(true);
		takeOffButton.addActionListener(l -> {
			controller.requestTakingOff();
		});

		landButton = new JButton("Land");
		landButton.setEnabled(true);
		landButton.addActionListener(l -> {
			controller.requestLanding();
		});

		final JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
		buttonPanel.add(takeOffButton);
		buttonPanel.add(landButton);

		mainPanel.add(buttonPanel);
		setContentPane(mainPanel);

		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent ev) {
				System.exit(-1);
			}
		});
	}

	public void display() {
		SwingUtilities.invokeLater(() -> {
			if (Objects.isNull(controller)) {
				logger.atError().log("Cannot display GUI: controller not setup yet.");
				System.exit(-1);
			}
			this.pack();
			this.setVisible(true);
		});
	}

	public void registerController(final DashboardController controller) {
		this.controller = Objects.requireNonNull(controller, "Cannot register null controller to the view!");
		logger.atInfo().log("Dashboard Controller registered.");
	}

	public void displayHangarState(final String state) {
		this.hangarStateDisplay.setText(state);
	}

	public void displayDroneState(final String state) {
		this.droneStateDisplay.setText(state);
	}

	public void enableActionButtons() {
		this.landButton.setEnabled(true);
		this.takeOffButton.setEnabled(true);
	}

	public void disableActionButtons() {
		this.landButton.setEnabled(false);
		this.takeOffButton.setEnabled(false);
	}

	public void displayDroneDistance(final String distance) {
		this.distanceDisplay.setText(distance);
	}

	public void notifyHangarInAlarm() {
		this.hangarInAlarmNotification.setVisible(true);
	}

	public void clearHangarInAlarmNotification() {
		this.hangarInAlarmNotification.setVisible(false);
	}

	public void clearDroneDistanceDisplay() {
		this.distanceDisplay.setText("");
	}
}
