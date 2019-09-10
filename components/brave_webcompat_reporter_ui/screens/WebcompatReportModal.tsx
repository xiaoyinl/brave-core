/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

// Components group
import ReportView from './ReportView'
import ConfirmationView from './ConfirmationView'
import { CardLayout } from '../components'

interface Props {
  siteUrl: string
  submitted: boolean
  onSubmitReport: () => void
  onClose: () => void
}

export default class WebcompatReportModal extends React.PureComponent<Props, {}> {
  constructor (props: Props) {
    super(props)
  }

  render () {
    const {
      siteUrl,
      submitted,
      onSubmitReport,
      onClose
    } = this.props
    return (
      <CardLayout>
        {submitted ? (
          <ConfirmationView/>
        ) : (
          <ReportView
            siteUrl={siteUrl}
            onSubmitReport={onSubmitReport}
            onClose={onClose}
          />
        )}
      </CardLayout>
    )
  }
}
