package it.unibo.sdh.impl.view;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

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

import it.unibo.sdh.impl.controller.DashboardController;

public class DashboardView extends JFrame {
    
    private JButton takeOffButton;
    private JButton landButton;
    private JButton resetButton;
    private JTextField distanceDisplay;
    private JTextField hangarStateDisplay;
    private JTextField droneStateDisplay;

    private DashboardController controller;

    public DashboardView() {
        super(".:: Drone Remote Unit — DASHBOARD ::.");

        setSize(600,200);
		this.setResizable(false);
		
		final JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		mainPanel.setBorder(BorderFactory.createEmptyBorder(0, 20, 0, 20));
		
		final JPanel stateViewPanel = new JPanel();
		stateViewPanel.setLayout(new GridLayout(1, 2));

		final JPanel hangarStateView = new JPanel();
		hangarStateView.setLayout(new BoxLayout(hangarStateView, BoxLayout.Y_AXIS));
		hangarStateView.add(new JLabel("HANGAR state"));
		hangarStateDisplay = new JTextField();
		hangarStateDisplay.setEditable(false);
		hangarStateDisplay.setPreferredSize(new Dimension(150,15));
		hangarStateView.add(hangarStateDisplay);

		final JPanel droneStateView = new JPanel();
		droneStateView.setLayout(new BoxLayout(droneStateView, BoxLayout.Y_AXIS));
		droneStateView.add(new JLabel("DRONE state"));
		droneStateDisplay = new JTextField();
		droneStateDisplay.setEditable(false);
		droneStateDisplay.setPreferredSize(new Dimension(150,15));
		droneStateView.add(droneStateDisplay);
		
		stateViewPanel.add(hangarStateView);
		stateViewPanel.add(droneStateView);
		mainPanel.add(stateViewPanel);
		mainPanel.add(new JSeparator());
		// mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		
		final JPanel infoLine = new JPanel();
		infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));
		distanceDisplay = new JTextField("-- mt.");
		distanceDisplay.setEditable(false);
		distanceDisplay.setPreferredSize(new Dimension(150,15));
		infoLine.add(new JLabel("Distance to ground: "));
		infoLine.add(distanceDisplay);
		
		mainPanel.add(infoLine);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		mainPanel.setPreferredSize(new Dimension(200,20));
        
		takeOffButton = new JButton("Take Off");
		takeOffButton.setEnabled(true);
		takeOffButton.addActionListener((l) -> {
            controller.takeOff();
        });

		landButton = new JButton("Land");
		landButton.setEnabled(true);
		landButton.addActionListener((l) -> {
            controller.land();
        });

		// TODO: Work with reset
		resetButton = new JButton("RESET");
		resetButton.setEnabled(true);
		resetButton.setBackground(Color.RED);
		// resetButton.setForeground(Color.WHITE);
		resetButton.addActionListener((l) -> {
            // controller.reset();
        });

		final JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));	    
		buttonPanel.add(takeOffButton);
		buttonPanel.add(landButton);
		buttonPanel.add(resetButton);
		
		mainPanel.add(buttonPanel);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		setContentPane(mainPanel);

        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent ev){
				System.exit(-1);
			}
        });
    }

    public void display() {
        SwingUtilities.invokeLater(() -> {
            this.setVisible(true);
        });
    }

    public void registerController(DashboardController controller) {
        this.controller = controller;
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
}
