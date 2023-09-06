


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    if len(sys.argv) < 2:
        _pulse_logger.error("Expected inputs : <filename base path where there is a csv and log file by the name>")
        # For example path/to/scenario/results/Male_44yr_180.34cm_23.71bmi_0.21bff_72bpm_87mmHg_40.5mmHg_12bpm
        sys.exit(1)


    csv_file = Path(sys.argv[1] + ".csv")
    log_file = Path(sys.argv[1] + ".log")


    if not csv_file.is_file():
        _pulse_logger.error("Could not find csv file: "+str(csv_file))
        sys.exit(1)
    if not log_file.is_file():
        _pulse_logger.error("Could not find log file: "+str(log_file))
        sys.exit(1)

    sys_tgts, patient_tgts = gen_targets(log_file)
    # sys_tgts is a SETimeSeriesValidationTargetMap
    run_validation(csv_file, sys_tgts) # -> SEPropertyValidationMap
    # Generate Tables (Optional)
    # run_validation(csv_file, patient_tgts)